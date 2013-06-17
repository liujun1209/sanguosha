#include "SgsClient.h"
#include "Constants.h"
#include "PacketFactory.h"
#include "MyLogger.h"
#include "WaitingRoomScene.h"
#include "LobbyScene.h"
#include "UIThreadMQ.h"
#include "ClientPlayer.h"
#include "SgsUtil.h"
#include "TableScene.h"

using namespace sgs::utils;
using namespace sgs::scene;


namespace sgs {

    namespace client {

        SgsClient::SgsClient(bool host):host_(host), status_(Waiting), waitingRoomPtr_(NULL),lobbyPtr_(NULL),playerTablePtr_(NULL),timer_(1)
        {

        }


        SgsClient::~SgsClient(void)
        {
        }

        void SgsClient::init() {
            client_ = boost::shared_ptr<AsioClient>(new AsioClient());
            client_->setIoHandler(shared_from_this());
        }

        void  SgsClient::connect(string ip) {
            CCLOG("SgsClient::connect ip = %s", ip.c_str() );
            boost::thread thread(boost::bind(&AsioClient::connect, client_, ip, NET_SERVER_PORT, 1000));
        }

        void  SgsClient::disconnect() {
            client_->disconnect();
        }

        void SgsClient::connectionClosed(const boost::shared_ptr<Connection> connectionPtr) {
            XmlIoHandler::connectionClosed(connectionPtr);
            //关闭心跳

        }

        void  SgsClient::connectionOpened(const boost::shared_ptr<Connection> connectionPtr) {
            LOG_DEBUG("SgsClient::connectionOpened cid = %d", connectionPtr->getId());
            XmlIoHandler::connectionOpened(connectionPtr);
            connectionPtr->write(sgs::packet::streamBeginPacket());
            connectionPtr->write(sgs::packet::client::joinRoomPacket());
            //开始心跳
            LOG_DEBUG("SgsClient::connectionOpened start heartBeat", connectionPtr->getId());
            timer_.addScheduleRepeatedTask(boost::bind(&SgsClient::heartBeat, this), 60000);
            boost::thread thread(boost::bind(&Timer::start, &timer_));
        }

        void SgsClient::startGame() {
            sendPacket(sgs::packet::gameStartPacket());
        }

        void SgsClient::heartBeat() {
            sendPacket(sgs::packet::heartBeatPacket());
        }

        void SgsClient::ready() {
            sendPacket(sgs::packet::client::readyPacket());
        }

        void SgsClient::sendPacket(const string& packet) {
            ConnectionPtr connectionPtr = client_->getConnectionPtr();
            if(connectionPtr) {
                connectionPtr->write(packet);
            }
            else {
                LOG_ERROR("SgsClient::sendPacket connectionPtr is null");
            }
            
        }

        void SgsClient::roomInfo(xmlDocumentPtr& doc) {
            pugi::xml_node node = doc->first_child();
            pugi::xpath_node_set nodeset = node.select_nodes(sgs::packet::PACKET_NAME_ROOM_PLAYER_PATH);
            for (pugi::xpath_node_set::const_iterator it = nodeset.begin();
                it !=  nodeset.end(); ++it)
            {
                pugi::xpath_node node = *it;
                pugi::xml_attribute idAtt = node.node().attribute(sgs::packet::PACKET_ATTRIBUTE_ID);
                int id = idAtt.as_int();
                pugi::xml_attribute nameAtt = node.node().attribute(sgs::packet::PACKET_ATTRIBUTE_NAME);
                string name(nameAtt.value());
                if(playerMap_.find(id) == playerMap_.end()) {
                    boost::shared_ptr<ClientPlayer> player(new ClientPlayer());
                    player->setId(id);
                    playerMap_.insert(pair<int, boost::shared_ptr<ClientPlayer> >(id, player));
                }
                if(waitingRoomPtr_) {
                    ADD_MESSAGE(boost::bind(&WaitingRoom::playerJoin, waitingRoomPtr_, id, name));
                }
            }
        }

        void SgsClient::playerJoin(xmlDocumentPtr& doc) {
            pugi::xml_node node = doc->first_child();
            pugi::xml_attribute idAtt = node.attribute(sgs::packet::PACKET_ATTRIBUTE_ID);
            int id = idAtt.as_int();
            pugi::xml_attribute nameAtt = node.attribute(sgs::packet::PACKET_ATTRIBUTE_NAME);
            string name(nameAtt.value());
            if(playerMap_.find(id) == playerMap_.end()) {
                boost::shared_ptr<ClientPlayer> player(new ClientPlayer());
                player->setId(id);
                playerMap_.insert(pair<int, boost::shared_ptr<ClientPlayer> >(id, player));
            }
            if(waitingRoomPtr_) {
                ADD_MESSAGE(boost::bind(&WaitingRoom::playerJoin, waitingRoomPtr_, id, name));
            }
        }

        void SgsClient::playerExit(xmlDocumentPtr& doc) {
            pugi::xml_node node = doc->first_child();
            pugi::xml_attribute idAtt = node.attribute(sgs::packet::PACKET_ATTRIBUTE_ID);
            int id = idAtt.as_int();
            //
            map<int, boost::shared_ptr<ClientPlayer> >::iterator it = playerMap_.find(id);
            if(it != playerMap_.end()) {
                playerMap_.erase(it);
            }
            if(waitingRoomPtr_) {
                ADD_MESSAGE(boost::bind(&WaitingRoom::playerExit, waitingRoomPtr_, id));
                //waitingRoomPtr_->playerExit(id);
            }
        }

        void SgsClient::requestRoomInfo() {
            sendPacket(sgs::packet::client::requestRoomInfoPacket());
        }

        void SgsClient::messageSent(const boost::shared_ptr<Connection> connectionPtr, const std::string& msg) {
            LOG_DEBUG("SgsClient::messageSent: cid = %d, msg = %s" , connectionPtr->getId() , msg.c_str());
        }


        void  SgsClient::messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc) {
            pugi::xml_node node = doc->first_child();
            //TODO 修复bug NODE的first_child为空
            if(node.name() == NULL || node.name() == "" ) {
                CCLog("node name is null!-===========================================");
                return;
            }
            if(!equals(node.name(), sgs::packet::PACKET_NAME_HEARTBEAT)) {
                //debug========
                stringstream ss;
                doc->print(ss,"\t",pugi::format_raw);
                LOG_DEBUG("SgsClient::messaged received: cid = %d,msg = %s" , connectionPtr->getId() , ss.str().c_str());
                //end=============
            }

            if(status_ == Waiting) {
                //连接失败
                if(equals(node.name(), sgs::packet::PACKET_NAME_ERROR)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_NAME_ERROR");
                    return;
                }
                //认证通过
                if(equals(node.name(), sgs::packet::PACKET_NAME_JOIN_ROOM_SUCCESS)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_NAME_JOIN_ROOM_SUCCESS");
                    int id = node.attribute(sgs::packet::PACKET_ATTRIBUTE_ID).as_int();
                    controlPlayerId_ = id;
                    boost::shared_ptr<ControlClientPlayer> player(new ControlClientPlayer());
                    player->setId(id);
                    player->setSgsClient(boost::dynamic_pointer_cast<SgsClient, XmlIoHandler>(shared_from_this()));
                    playerMap_.insert(pair<int, boost::shared_ptr<ClientPlayer> >(id, player));
                    if(!host_) {
                        if(lobbyPtr_){
                            //非主客户端通知大厅切换至等待页面
                            ADD_MESSAGE(boost::bind(&Lobby::authSuccessCallback, lobbyPtr_));
                        }
                    }
                    else {
                        //直接请求
                        this->requestRoomInfo();
                    }
                    return;
                }
                //房间信息
                if(equals(node.name(), sgs::packet::PACKET_NAME_ROOM)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_NAME_ROOM");
                    this->roomInfo(doc);
                    return;
                }
                //新用户加入
                if(equals(node.name(), sgs::packet::PACKET_NAME_PLAYER_JOIN)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_NAME_PLAYER_JOIN");
                    this->playerJoin(doc);
                    return;
                }
                //玩家退出
                if(equals(node.name(), sgs::packet::PACKET_NAME_PLAYER_EXIT)) {
                    
                    LOG_DEBUG("SgsClient::messaged PACKET_NAME_PLAYER_EXIT");
                    this->playerExit(doc);
                    return;
                }
                //游戏开始
                if(equals(node.name(), sgs::packet::PACKET_GAME_START)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_GAME_START");
                    if(waitingRoomPtr_) {
                        ADD_MESSAGE(boost::bind(&WaitingRoom::startGameCallback, waitingRoomPtr_));
                    }
                    return;
                }
                //位置和角色
                if(equals(node.name(), sgs::packet::PACKET_NAME_SEAT_AND_ROLE)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_NAME_SEAT_AND_ROLE");
                    updateSeatAndRole(doc);
                    return;
                }
                if(equals(node.name(), sgs::packet::PACKET_PLAYER_MESSAGE)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_PLAYER_MESSAGE");
                    int id = node.attribute(sgs::packet::PACKET_ATTRIBUTE_ID).as_int();
                    map<int, boost::shared_ptr<ClientPlayer> >::iterator it = playerMap_.find(id);
                    if(it != playerMap_.end()) {
                        it->second->execute(doc);
                    }
                    return;
                }
                //重置剩余牌数
                if(equals(node.name(), sgs::packet::PACKET_NAME_LEFT_PILES)) {
                    LOG_DEBUG("SgsClient::messaged PACKET_NAME_LEFT_PILES");
                    int num = node.attribute(sgs::packet::PACKET_ATTRIBUTE_NUM).as_int();
                    if(playerTablePtr_) {
                        ADD_MESSAGE(boost::bind(&PlayerTable::setLeftPiles, playerTablePtr_, num));
                    }
                    return;
                }
            }
        }

        int SgsClient::getPlayerCount() {
            return playerMap_.size();
        }

        void SgsClient::updateSeatAndRole(xmlDocumentPtr& doc) {
            LOG_DEBUG("SgsClient::updateSeatAndRole");
            pugi::xml_node node = doc->first_child();

            pugi::xml_attribute lordAtt = node.attribute(sgs::packet::PACKET_ATTRIBUTE_LORD);
            int lord = lordAtt.as_int();

            pugi::xml_attribute loyalistAtt = node.attribute(sgs::packet::PACKET_ATTRIBUTE_LOYALIST);
            int loyalist = loyalistAtt.as_int();

            pugi::xml_attribute rebelAtt = node.attribute(sgs::packet::PACKET_ATTRIBUTE_REBEL);
            int rebel = rebelAtt.as_int();

            pugi::xml_attribute renegadeAtt = node.attribute(sgs::packet::PACKET_ATTRIBUTE_RENEGADE);
            int renegade = renegadeAtt.as_int();

            if(playerTablePtr_) {
                ADD_MESSAGE(boost::bind(&PlayerTable::setRoleInfo, playerTablePtr_, lord, loyalist, rebel, renegade));
            }

            pugi::xpath_node_set nodeset = node.select_nodes(sgs::packet::PACKET_NAME_SEAT_AND_ROLE_PLAYER_PATH);
            boost::shared_ptr<ClientPlayer> first;
            boost::shared_ptr<ClientPlayer> last;
            int seat = 0;
            for (pugi::xpath_node_set::const_iterator it = nodeset.begin();
                it !=  nodeset.end(); ++it)
            {
                pugi::xpath_node node = *it;
                pugi::xml_attribute idAtt = node.node().attribute(sgs::packet::PACKET_ATTRIBUTE_ID);
                int id = idAtt.as_int();
                map<int, boost::shared_ptr<ClientPlayer> >::iterator playerIterator = playerMap_.find(id);
                if(playerIterator != playerMap_.end()) {
                    if(!first) {
                        //初始化
                        first = playerIterator->second;
                    }
                    else {
                        last->setNextPlayer(playerIterator->second);
                        last->setNextLivePlayer(playerIterator->second);
                        playerIterator->second->setPrePlayer(last);
                        playerIterator->second->setPreLivePlayer(last);
                    }
                    //关联数据与视图
                    playerIterator->second->setPlayerUI(playerTablePtr_->getPlayerSeat(seat++));

                    pugi::xml_attribute roleAtt = node.node().attribute(sgs::packet::PACKET_ATTRIBUTE_ROLE);
                    if(roleAtt) {
                        //有角色
                        Player::Role role = sgs::utils::getRoleFromString(string(roleAtt.value()));
                        playerIterator->second->setRole(role);
                    }
                    last = playerIterator->second;
                }
            }
            last->setNextPlayer(first);
            last->setNextLivePlayer(first);
            first->setPrePlayer(last);
            first->setPreLivePlayer(last);
        }

    }
}