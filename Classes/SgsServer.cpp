#include <boost\bind.hpp>
#include <boost\foreach.hpp>
#include <string.h>
#include "SgsServer.h"
#include "Constants.h"
#include "PacketFactory.h"
#include "Config.h"
#include "MyLock.h"
#include "StringResource.h"
#include "MyLogger.h"
#include "Engine.h"
#include <boost\algorithm\string.hpp>

using namespace sgs::utils;
using namespace sgs::packet;

namespace sgs {
    namespace server {

        static const int LORD_MAX_GENERAL = 5;
        static const int MAX_GENERAL = 3;
        static const int INIT_CARD_NUMBER = 4;

        static const int PHASE_CHANGE_WAIT_TIME = 1000;//ms

        Player::Role role_init_value_2[] = {Player::Lord, Player::Rebel};
        Player::Role role_init_value_3[] = {Player::Lord, Player::Rebel, Player::Renegade};
        Player::Role role_init_value_4[] = {Player::Lord, Player::Loyalist, Player::Rebel, Player::Renegade};
        Player::Role role_init_value_5[] = {Player::Lord, Player::Loyalist, Player::Rebel, Player::Rebel, Player::Renegade};
        Player::Role role_init_value_6[] = {Player::Lord, Player::Loyalist, Player::Rebel, Player::Rebel, Player::Rebel, Player::Renegade};
        Player::Role role_init_value_7[] = {Player::Lord, Player::Loyalist ,Player::Loyalist, Player::Rebel,Player::Rebel,Player::Rebel, Player::Renegade};
        Player::Role role_init_value_8[] = {Player::Lord, Player::Loyalist ,Player::Loyalist, Player::Rebel,Player::Rebel,Player::Rebel,Player::Rebel, Player::Renegade};
        
        map<int, Player::Role* >::value_type init_value[] =
        {
            pair<int, Player::Role*  >(2, role_init_value_2),
            pair<int, Player::Role*  >(3, role_init_value_3),
            pair<int, Player::Role*  >(4, role_init_value_4),
            pair<int, Player::Role*  >(5, role_init_value_5),
            pair<int, Player::Role*  >(6, role_init_value_6),
            pair<int, Player::Role*  >(7, role_init_value_7),
            pair<int, Player::Role*  >(8, role_init_value_8)
        };

        static const map<int, Player::Role* > PLAYER_ROLE_MAP(init_value, init_value + 7);

        SgsServer::SgsServer(void) : status_(Waiting), readyCount_(0), mainLoopTimer_(1)
        {
            
        }


        SgsServer::~SgsServer(void)
        {
        }

        void SgsServer::init() {
            server_ = boost::shared_ptr<AsioServer>(new AsioServer());
            server_->setIoHandler(XmlIoHandler::shared_from_this());
            server_->setPort(NET_SERVER_PORT);
        }

        void SgsServer::start() {
            boost::thread tcpThread(boost::bind(&AsioServer::bind, server_));
            boost::thread boardcastThread(boost::bind(&AsioServer::startBoardcastLocalIp, server_));
        }

        void SgsServer::stop() {
            server_->stopBoardcastLocalIp();
            server_->unbind();
        }

        void SgsServer::connectionOpened(const boost::shared_ptr<Connection> connectionPtr) {
            XmlIoHandler::connectionOpened(connectionPtr);
            connectionPtr->write("<stream:stream>");
        }

        void SgsServer::connectionClosed(const boost::shared_ptr<Connection> connectionPtr) {
            if(status_ == Waiting) {
                //等待
                boost::shared_ptr<ServerPlayer> player;
                {
                    WriteLock lock(mutex_);
                    vector<int >::iterator it;
                    for(it = playerList_.begin(); it != playerList_.end(); it++) {
                        boost::shared_ptr<ServerPlayer> tempPlayer = playerMap_[*it];
                        if(tempPlayer->getConnectionId() == connectionPtr->getId()) {
                            player = tempPlayer;
                            playerList_.erase(it);
                            playerMap_.erase(*it);
                            break;
                        }
                    }
                }
                if(player) {
                    broadcastPacket(sgs::packet::server::deletePlayerPacket(player));
                }
            }
        }

        void SgsServer::messageSent(const boost::shared_ptr<Connection> connectionPtr, const std::string& msg) {
            LOG_DEBUG("Server sent: cid =%d,,msg = %s" , connectionPtr->getId() ,msg.c_str());
        }
            
        int SgsServer::initCards() {
            WriteLock lock(cardMutex_);
            cardIdList_ = Engine::getInstance()->getAllCardsId();
            std::random_shuffle(cardIdList_.begin() , cardIdList_.end());
            return cardIdList_.size();
        }

        void SgsServer::discardCards(const set<int>& discardCards) {
            WriteLock lock(cardMutex_);
            BOOST_FOREACH(int cardId, discardCards) {
                discardCardIdList_.push_back(cardId);
            }
        }

        vector<int> SgsServer::drawCards(int num) {
            WriteLock lock(cardMutex_);
            int size = cardIdList_.size();
            if(size < num) {
                std::random_shuffle(discardCardIdList_.begin() , discardCardIdList_.end());
                for(int i = 0; i< size;i ++ ) {
                    discardCardIdList_.push_back(cardIdList_.at(i));
                }
                cardIdList_ = discardCardIdList_;
                discardCardIdList_.clear();
                
                broadcastPacket(sgs::packet::server::leftPilesPacket(cardIdList_.size()));
            }

            vector<int> cards;
            for(int i =0 ;i < num; i++) {
                cards.push_back(cardIdList_.back());
                cardIdList_.pop_back();
            }
            
            return cards;
        }

        void SgsServer::messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc) {
            pugi::xml_node node = doc->first_child();
            if(node.name() == NULL || node.name() == "" ) {
                CCLog("node name is null!");
                return;
            }
            if(equals((doc->first_child()).name(), PACKET_NAME_HEARTBEAT)) {
                //心跳
                connectionPtr->write(sgs::packet::heartBeatPacket());
                return;
            }
            else {
                //debug========
                stringstream ss;
                doc->print(ss,"\t",pugi::format_raw);
                LOG_DEBUG("Server received: cid = %d, msg = %s" , connectionPtr->getId(), ss.str().c_str());
                //end=============
            }
            if(status_ == Waiting) {
                //用户连接信息
                if(equals(node.name(), PACKET_NAME_PLAYER)) {
                    if(validate(connectionPtr, doc)) {
                        boost::shared_ptr<ServerPlayer> player(new ServerPlayer());
                        pugi::xml_attribute nameAtt = doc->first_child().attribute(PACKET_ATTRIBUTE_NAME);
                        player->setName(nameAtt.value());
                        {   
                            WriteLock lock(mutex_);
                            player->setConnectionId(connectionPtr->getId());
                            playerList_.push_back(player->getId());
                            playerMap_.insert(pair<int, boost::shared_ptr<ServerPlayer> >(player->getId(), player));
                            //给用户自己id信息
                            connectionPtr->write(sgs::packet::server::joinSuccessPacket(player));
                        }
                        player->setSgsServer(boost::shared_dynamic_cast<SgsServer,XmlIoHandler>(shared_from_this()));
                        //给其他用户广播新用户信息
                        broadcastPacketExclude(player , sgs::packet::server::newPlayerPacket(player));
                    }
                    return;
                }
                //请求房间信息
                if(equals(node.name(), PACKET_NAME_REQUEST_ROOM)) {
                    //getPlayerByConnectionId(connectionPtr->getId());
                    vector<boost::shared_ptr<ServerPlayer> > tempPlayerList;
                    {
                        WriteLock lock(mutex_);
                        BOOST_FOREACH(int id ,playerList_) {
                            tempPlayerList.push_back(playerMap_[id]);
                        }
                    }
                    connectionPtr->write(sgs::packet::server::playerListPacket(tempPlayerList));
                    return;
                }
                //游戏开始
                if(equals(node.name(), PACKET_GAME_START)) {
                    status_ = Gaming;
                    broadcastPacket(sgs::packet::gameStartPacket());
                    //启动计时器
                    return;
                }

            }
            else {
                //游戏准备
                if(equals(node.name(), PACKET_GAME_READY)) {
                    //
                    {
                        WriteLock lock(mutex_);
                        readyCount_++;
                    }
                    if(readyCount_ == playerList_.size()) {
                        //所有玩家准备完毕
                        //分配位置和角色
                        assignSeatAndRole();
                        //分配给主公武将
                        assignLordGeneral();
                    }
                    return;
                }
                //玩家消息
                if(equals(node.name(), sgs::packet::PACKET_PLAYER_MESSAGE)) {

                    int id = node.attribute(sgs::packet::PACKET_ATTRIBUTE_ID).as_int();
                    map<int, boost::shared_ptr<ServerPlayer> >::iterator it = playerMap_.find(id);
                    if(it != playerMap_.end()) {
                       it->second->execute(doc);
                    }
                    return;
                }
                //玩家信息
            }
        }

        void SgsServer::assignSeatAndRole() {
            srand ( unsigned ( time (NULL) ) );
            int size = playerList_.size();
            //随机角色
            random_shuffle(playerList_.begin(), playerList_.end()); 

            Player::Role * roles = PLAYER_ROLE_MAP.find(size)->second;
            int lord = 0 ,loyalist = 0 ,rebel = 0 , renegade = 0;
            for(int i = 0; i< size; i++) {
                Player::Role role = *(roles +i);
                playerMap_[playerList_.at(i)]->setRole(role);
                switch(role) {
                case Player::Lord:
                    lord++;
                    break;
                case Player::Loyalist:
                    loyalist++;
                    break;
                case Player::Rebel:
                    rebel++;
                    break;
                case Player::Renegade:
                    renegade++;
                    break;
                }
            }
            //0位置为主公
            lordPlayer_ =  playerMap_[playerList_.at(0)];
            //随机位置
            random_shuffle(playerList_.begin(), playerList_.end()); 

            for(int i = 0; i< size - 1 ; i++) {
                 playerMap_[playerList_.at(i)]->setNextPlayer(playerMap_[playerList_.at(i+1)]);
                playerMap_[playerList_.at(i)]->setNextLivePlayer(playerMap_[playerList_.at(i+1)]);
                
                playerMap_[playerList_.at(i+1)]->setPrePlayer(playerMap_[playerList_.at(i)]);
                playerMap_[playerList_.at(i+1)]->setPreLivePlayer(playerMap_[playerList_.at(i)]);
            }
            int last = size-1;
            playerMap_[playerList_.at(last)]->setNextPlayer(playerMap_[playerList_.at(0)]);
            playerMap_[playerList_.at(last)]->setNextLivePlayer(playerMap_[playerList_.at(0)]);
            playerMap_[playerList_.at(0)]->setPrePlayer(playerMap_[playerList_.at(last)]);
            playerMap_[playerList_.at(0)]->setPreLivePlayer(playerMap_[playerList_.at(last)]);

            BOOST_FOREACH(int playerId, playerList_ ) {
                playerMap_[playerId]->assignSeatAndRole(lord, loyalist, rebel, renegade);
            }
            
        }

        void SgsServer::assignLordGeneral() {
            vector<string> lordList = Engine::getInstance()->getLordList();
            BOOST_FOREACH(string name, lordList) {
                alreadyAssignGeneral_.insert(name);
            }
            int size = lordList.size();
            if(size < LORD_MAX_GENERAL) {
                vector<string> generalList = Engine::getInstance()->getRandomGenerals(LORD_MAX_GENERAL- size, alreadyAssignGeneral_);
                BOOST_FOREACH(string name, generalList) {
                    lordList.push_back(name);
                    alreadyAssignGeneral_.insert(name);
                }
            }
            lordPlayer_->askChooseLordGeneral(lordList);
            //清空
            alreadyAssignGeneral_.clear();
        }
        
        void SgsServer::assignOtherGeneral(const string & lordName) {
            alreadyAssignGeneral_.insert(lordName);
            BOOST_FOREACH(int playerId , playerList_) {
                boost::shared_ptr<ServerPlayer> player= playerMap_[playerId];
                if(player != lordPlayer_) {
                    vector<string> generalList = Engine::getInstance()->getRandomGenerals(MAX_GENERAL, alreadyAssignGeneral_);
                    BOOST_FOREACH(string name, generalList) {
                        alreadyAssignGeneral_.insert(name);
                    }
                    player->askChooseGeneral(lordName, generalList);
                }
            }
        }

        void SgsServer::checkChooseGeneral() {
            
            BOOST_FOREACH(int playerId , playerList_) {
                boost::shared_ptr<ServerPlayer> player= playerMap_[playerId];
                if(!player->getGeneral()){
                    //未选武将
                    return;
                }
            }
            //所有人都选完武将
            BOOST_FOREACH(int playerId , playerList_) {
                boost::shared_ptr<ServerPlayer> player = playerMap_[playerId];
                BOOST_FOREACH(int tempId , playerList_) {
                    boost::shared_ptr<ServerPlayer> tempPlayer = playerMap_[tempId];
                    if(tempPlayer != player && tempPlayer->getRole() != Player::Lord) {
                        sendPacket(player, sgs::packet::client::selectGeneralPacket(tempPlayer, tempPlayer->getGeneral()));
                    }
                }
            }
            //初始化洗牌
            int count = initCards();
            broadcastPacket(sgs::packet::server::leftPilesPacket(count));
            //发底牌
            boost::shared_ptr<ServerPlayer> currentPlayer = lordPlayer_;
            do {
                
                currentPlayer->drawCards(drawCards(INIT_CARD_NUMBER));

                currentPlayer = boost::dynamic_pointer_cast<ServerPlayer, Player>(currentPlayer->getNextLivePlayer());
            }while(currentPlayer != lordPlayer_);

            currentPlayer_ = lordPlayer_;
            
            //1s后开始游戏主流程
            mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, false), 1000);
            boost::thread thread(boost::bind(&Timer::start, &mainLoopTimer_));
        }

        void SgsServer::mainLoop(bool timerCall) {
            LOG_DEBUG("=====mainLoop in======");
            //清空定时器
            mainLoopTimer_.cleanAllTask();

            Player::Phase phase = currentPlayer_->getPhase();
            //回合开始
            if(phase == Player::PhaseNone) {
                LOG_DEBUG("id = %d, phase = %d" , currentPlayer_->getId() , phase);
                phase = Player::Start;
                currentPlayer_->setPhase(phase);
                
            }
            //开始阶段
            if(phase == Player::Start) {
                LOG_DEBUG("id = %d ,phase = %d" , currentPlayer_->getId() , phase);
                phase = Player::Judge;
                currentPlayer_->setPhase(phase);
                
                mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, false), PHASE_CHANGE_WAIT_TIME);
                return;
            }
            //判定阶段
             if(phase == Player::Judge) {
                LOG_DEBUG("id = %d ,phase = %d" , currentPlayer_->getId() , phase);
                vector<boost::shared_ptr<Card> > judaeCards = currentPlayer_->getJudgingArea();
                if(judaeCards.size() != 0) {
                    //走判定流程
                    return;
                }
                phase = Player::Draw;
                currentPlayer_->setPhase(phase);
                //抓牌
                currentPlayer_->drawCards(drawCards(2));
                //
                mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, false), PHASE_CHANGE_WAIT_TIME);
                return;
            }
            //抓牌阶段
            if(phase == Player::Draw) {
                LOG_DEBUG("id = %d ,phase = %d" , currentPlayer_->getId() , phase);
                //什么也不做
                phase = Player::Play;
                currentPlayer_->setPhase(phase);
                mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, false), PHASE_CHANGE_WAIT_TIME);
                return;
            }
            //出牌阶段
            if(phase == Player::Play) {
                
                LOG_DEBUG("id = %d ,phase = %d" , currentPlayer_->getId() , phase);
                if(timerCall) {
                    phase = Player::Discard;
                    currentPlayer_->setPhase(phase);
                    mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, false), PHASE_CHANGE_WAIT_TIME);
                    return;
                }
                else {
                    mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, true), 15000);
                    currentPlayer_->showTimeBar(15);
                    return;
                }
            }
            //弃牌阶段
            if(phase == Player::Discard) {
                if(!currentPlayer_->need2Discard()){
                    phase = Player::Finish;
                    currentPlayer_->setPhase(phase);
                    //不需要弃牌
                    return;
                }
                LOG_DEBUG("id = %d ,phase = %d" , currentPlayer_->getId() , phase);
                if(timerCall) {
                    currentPlayer_->forceDiscard();
                    phase = Player::Finish;
                    currentPlayer_->setPhase(phase);
                }
                else {
                    mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, true), 15000);
                    currentPlayer_->showTimeBar(15);
                    return;
                }
            }
            //单人回合结束
            if(phase == Player::Finish || phase == Player::NotActive) {
                LOG_DEBUG("id = %d ,phase = %d" , currentPlayer_->getId() , phase);
                
                currentPlayer_->setPhase(Player::PhaseNone);
                currentPlayer_ = boost::dynamic_pointer_cast<ServerPlayer,Player>(currentPlayer_->getNextLivePlayer());
                mainLoopTimer_.addTask(boost::bind(&SgsServer::mainLoop, this, false), 1000);
            }
            
            LOG_DEBUG("=====mainLoop out======");
        }


        bool SgsServer::validate(const boost::shared_ptr<Connection> connectionPtr ,xmlDocumentPtr& doc) {
            if(playerList_.size() > MAX_PLAYER ){
                //用户已满
                string msg = sgs::packet::server::errorMsgPacket(ERROR_CODE_ROOM_FULL, STRING_RESOURCE(ERROR_CODE_ROOM_FULL));
                connectionPtr->write(msg);
                return false;
            }
            pugi::xml_attribute versionAtt = doc->first_child().attribute(PACKET_ATTRIBUTE_VERSION);
            if(!equals(versionAtt.value(),
                Config::getInstance()->getProperty(CONFIG_KEY_VERSION).c_str())){
                    //版本不相符
                    string msg = sgs::packet::server::errorMsgPacket(ERROR_CODE_VERSION_INVALIDATE, STRING_RESOURCE(ERROR_CODE_VERSION_INVALIDATE));
                    connectionPtr->write(msg);
                    return false;
            }
            return true;
        }

        void SgsServer::sendPacket(boost::shared_ptr<ServerPlayer> player, const string& packet) {
           ConnectionPtr connection = server_->getConnection(player->getConnectionId());
           if(connection) {
               connection->write(packet);
           }
           else {
                LOG_ERROR("SgsServer::sendPacket connection is null");
           }
               
        }

        void SgsServer::broadcastPacket(const string& packet) {
            //
            ReadLock lock(mutex_);
            vector<int >::iterator it;
            for(it = playerList_.begin(); it != playerList_.end(); it++) {
                sendPacket(playerMap_[*it] , packet);
                //server_->getConnection((*it)->getConnectionId())->write(packet);
            }
        }

        void SgsServer::broadcastPacketExclude(boost::shared_ptr<ServerPlayer> player, const string& packet) {
            ReadLock lock(mutex_);
            vector<int >::iterator it;
            for(it = playerList_.begin(); it != playerList_.end(); it++) {
                if(player != playerMap_[*it]){
                    sendPacket(playerMap_[*it] , packet);
                    //server_->getConnection((*it)->getConnectionId())->write(packet);
                }
            }
        }

        boost::shared_ptr<ServerPlayer> SgsServer::getPlayerByConnectionId(long connectionId) {
            ReadLock lock(mutex_);
            vector<int >::iterator it;
            for(it = playerList_.begin(); it != playerList_.end(); it++) {
                if((playerMap_[*it])->getConnectionId() == connectionId){
                    return playerMap_[*it];
                }
            }
            return boost::shared_ptr<ServerPlayer>();
        }

    }
}