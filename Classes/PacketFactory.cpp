#include "PacketFactory.h"
#include <boost\foreach.hpp>
#include "pugixml.hpp"
#include "Config.h"
#include "Constants.h"
#include "SgsUtil.h"

using namespace pugi;

using namespace sgs::utils;
using namespace sgs::packet;

namespace sgs{
    namespace packet {
        /**
        * @brief stream开始报文
        **/
        string streamBeginPacket() {
            return "<stream:stream>";
        }

        /**
        * @brief stream结束报文
        **/
        string streamEndPacket() {
            return "</stream:stream>";
        }

        /**
        * @brief 游戏开始报文
        **/
        string gameStartPacket() {
            return "<GameStart/>";
        }

        /**
        * @brief 心跳报文
        **/
        string heartBeatPacket() {
            pugi::xml_document doc;
            pugi::xml_node node = doc.append_child(PACKET_NAME_HEARTBEAT);
            stringstream ss;
            node.print(ss,"\t",pugi::format_raw);
            return ss.str();
        }

        namespace client {
            /**
            * @brief 加入房间报文
            **/
            string joinRoomPacket() {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_PLAYER);

                pugi::xml_attribute nameAtt = node.append_attribute(PACKET_ATTRIBUTE_NAME);
                nameAtt.set_value(CONFIG_PROPERTY(CONFIG_KEY_NAME).c_str());

                pugi::xml_attribute versionAtt = node.append_attribute(PACKET_ATTRIBUTE_VERSION);
                versionAtt.set_value(CONFIG_PROPERTY(CONFIG_KEY_VERSION).c_str());

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            /**
            * @brief 请求房间信息报文
            **/
            string requestRoomInfoPacket() {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_REQUEST_ROOM);

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string readyPacket() {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_GAME_READY);

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string selectGeneralPacket(boost::shared_ptr<Player> player, boost::shared_ptr<General> general) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_CHOOSE_GENERAL);

                pugi::xml_attribute name = node.append_attribute(PACKET_ATTRIBUTE_NAME);
                name.set_value(general->getName().c_str());

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }
        }

        namespace server {

            string forceDiscardCards(const boost::shared_ptr<ServerPlayer> player, const set<int>& cards) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_FORCE_DISCARD_CARD);

                pugi::xml_attribute numAtt = playerMessage.append_attribute(PACKET_ATTRIBUTE_NUM);
                numAtt.set_value(cards.size());

                BOOST_FOREACH(int cardId, cards) {
                    pugi::xml_node cardNode = node.append_child(PACKET_NAME_CARD);
                    
                    pugi::xml_attribute id = cardNode.append_attribute(PACKET_ATTRIBUTE_ID);
                    id.set_value(cardId);
                }

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string discardCards(const boost::shared_ptr<ServerPlayer> player, const set<int>& cards) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_DISCARD_CARD);

                pugi::xml_attribute numAtt = playerMessage.append_attribute(PACKET_ATTRIBUTE_NUM);
                numAtt.set_value(cards.size());

                BOOST_FOREACH(int cardId, cards) {
                    pugi::xml_node cardNode = node.append_child(PACKET_NAME_CARD);
                    
                    pugi::xml_attribute id = cardNode.append_attribute(PACKET_ATTRIBUTE_ID);
                    id.set_value(cardId);
                }

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }


            string leftPilesPacket(const int number) {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_LEFT_PILES);

                pugi::xml_attribute id = node.append_attribute(PACKET_ATTRIBUTE_NUM);
                id.set_value(number);

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            /**
            * @brife 显示时间条
            **/
            string showTimeBarPacket(const boost::shared_ptr<ServerPlayer> player, float time) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_SHOW_TIME_BAR);

                pugi::xml_attribute phaseAtt = node.append_attribute(PACKET_ATTRIBUTE_TIME);

                phaseAtt.set_value(time);

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            /**
            * @brife 隐藏时间条
            **/
            string hiddenTimeBarPacket(const boost::shared_ptr<ServerPlayer> player) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_HIDDEN_TIME_BAR);

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string phaseChangePacket(const boost::shared_ptr<ServerPlayer> player, const Player::Phase phase) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_PHASE_CHANGE);

                pugi::xml_attribute phaseAtt = node.append_attribute(PACKET_ATTRIBUTE_PHASE);

                phaseAtt.set_value(sgs::utils::getPhaseString(phase).c_str());

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string drawCardPacket(const boost::shared_ptr<ServerPlayer> player, const vector<int>& cardIds, bool cardDetail) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_DRAW_CARD);
                pugi::xml_attribute numAtt = node.append_attribute(PACKET_ATTRIBUTE_NUM);
                numAtt.set_value(cardIds.size());

                if(cardDetail) {
                    vector<int>::const_iterator it;
                    for(it = cardIds.begin(); it != cardIds.end(); it++) {
                        pugi::xml_node child = node.append_child(PACKET_NAME_CARD);
                        pugi::xml_attribute idAtt = child.append_attribute(PACKET_ATTRIBUTE_ID);
                        idAtt.set_value(*it);
                    }
                }

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string discardCardPacket(const boost::shared_ptr<ServerPlayer> player, const vector<int>& cardIds, bool cardDetail) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_DISCARD_CARD);
                pugi::xml_attribute numAtt = node.append_attribute(PACKET_ATTRIBUTE_NUM);
                numAtt.set_value(cardIds.size());

                if(cardDetail) {
                    vector<int>::const_iterator it;
                    for(it = cardIds.begin(); it != cardIds.end(); it++) {
                        pugi::xml_node child = node.append_child(PACKET_NAME_CARD);
                        pugi::xml_attribute idAtt = child.append_attribute(PACKET_ATTRIBUTE_ID);
                        idAtt.set_value(*it);
                    }
                }

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }


            string chooseLordGeneral(const boost::shared_ptr<ServerPlayer> player, const vector<string> & generals) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id = playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_CHOOSE_LORD);
                vector<string>::const_iterator it;
                for(it = generals.begin(); it != generals.end(); it++) {
                    pugi::xml_node child = node.append_child(PACKET_NAME_GENERAL);
                    pugi::xml_attribute name =child.append_attribute(PACKET_ATTRIBUTE_NAME);
                    name.set_value((*it).c_str());
                }

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string chooseGeneral(const boost::shared_ptr<ServerPlayer> player, const string & lord, const vector<string> & generals) {
                pugi::xml_document doc;
                pugi::xml_node playerMessage = doc.append_child(PACKET_PLAYER_MESSAGE);
                pugi::xml_attribute id =playerMessage.append_attribute(PACKET_ATTRIBUTE_ID);
                id.set_value(player->getId());

                pugi::xml_node node = playerMessage.append_child(PACKET_NAME_CHOOSE_GENERAL);

                pugi::xml_node lordNode = node.append_child(PACKET_NAME_LORD);
                pugi::xml_attribute name =lordNode.append_attribute(PACKET_ATTRIBUTE_NAME);
                name.set_value(lord.c_str());

                vector<string>::const_iterator it;
                for(it = generals.begin(); it != generals.end(); it++) {
                    pugi::xml_node child = node.append_child(PACKET_NAME_GENERAL);
                    pugi::xml_attribute name =child.append_attribute(PACKET_ATTRIBUTE_NAME);
                    name.set_value((*it).c_str());
                }

                stringstream ss;
                playerMessage.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            string seatAndRolePacket(const boost::shared_ptr<ServerPlayer> player, int lord, int loyalist, int rebel, int renegade) {
                boost::shared_ptr<ServerPlayer> curPlayer = player;
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_SEAT_AND_ROLE);

                pugi::xml_attribute lordAtt = node.append_attribute(PACKET_ATTRIBUTE_LORD);
                lordAtt.set_value(lord);

                pugi::xml_attribute loyalistAtt = node.append_attribute(PACKET_ATTRIBUTE_LOYALIST);
                loyalistAtt.set_value(loyalist);

                pugi::xml_attribute rebelAtt = node.append_attribute(PACKET_ATTRIBUTE_REBEL);
                rebelAtt.set_value(rebel);

                pugi::xml_attribute renegadeAtt = node.append_attribute(PACKET_ATTRIBUTE_RENEGADE);
                renegadeAtt.set_value(renegade);

                do {
                    pugi::xml_node child = node.append_child(PACKET_NAME_PLAYER);
                    pugi::xml_attribute codeId = child.append_attribute(PACKET_ATTRIBUTE_ID);
                    codeId.set_value(curPlayer->getId());
                    if(curPlayer == player || curPlayer->getRole() == Player::Lord) {
                        //自己身份和主公会给身份
                        pugi::xml_attribute role = child.append_attribute(PACKET_ATTRIBUTE_ROLE);
                        role.set_value(getRoleString(curPlayer->getRole()).c_str());
                    }

                    curPlayer = boost::shared_dynamic_cast<ServerPlayer, Player>(curPlayer->getNextPlayer());
                }while(curPlayer != player);

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            /**
            * @brief 用户加入房间成功
            **/
            string joinSuccessPacket(const boost::shared_ptr<ServerPlayer> player) {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_JOIN_ROOM_SUCCESS);

                pugi::xml_attribute codeId = node.append_attribute(PACKET_ATTRIBUTE_ID);
                codeId.set_value(player->getId());

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            /**
            * @brief 错误报文
            **/
            string errorMsgPacket(int code ,const string& msg) {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_ERROR);
                pugi::xml_attribute codeAtt = node.append_attribute(PACKET_ATTRIBUTE_CODE);
                codeAtt.set_value(code);
                pugi::xml_attribute msgAtt = node.append_attribute(PACKET_ATTRIBUTE_MSG);
                msgAtt.set_value(msg.c_str());

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            /**
            * @brief 玩家列表报文
            **/
            string playerListPacket(const vector<boost::shared_ptr<ServerPlayer> > playerList) {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_ROOM);
                vector<boost::shared_ptr<ServerPlayer> >::const_iterator it ;
                for(it = playerList.begin(); it != playerList.end(); it++ ) {
                    boost::shared_ptr<ServerPlayer> player = *it;
                    pugi::xml_node playerNode = node.append_child(PACKET_NAME_PLAYER);
                    pugi::xml_attribute nameAtt = playerNode.append_attribute(PACKET_ATTRIBUTE_NAME);
                    nameAtt.set_value(player->getName().c_str());
                    pugi::xml_attribute itAtt = playerNode.append_attribute(PACKET_ATTRIBUTE_ID);
                    itAtt.set_value(player->getId());
                    pugi::xml_attribute headAtt = playerNode.append_attribute(PACKET_ATTRIBUTE_HEAD);
                    headAtt.set_value(player->getHead().c_str());
                }

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();

            }


            /**
            * @brief 新玩家加入报文
            **/
            string newPlayerPacket(const boost::shared_ptr<ServerPlayer> player) {
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_PLAYER_JOIN);

                pugi::xml_attribute nameAtt = node.append_attribute(PACKET_ATTRIBUTE_NAME);
                nameAtt.set_value(player->getName().c_str());
                pugi::xml_attribute itAtt = node.append_attribute(PACKET_ATTRIBUTE_ID);
                itAtt.set_value(player->getId());
                pugi::xml_attribute headAtt = node.append_attribute(PACKET_ATTRIBUTE_HEAD);
                headAtt.set_value(player->getHead().c_str());

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }

            /**
            * @brief 玩家退出报文
            **/
            string deletePlayerPacket(const boost::shared_ptr<ServerPlayer> player){
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_PLAYER_EXIT);

                pugi::xml_attribute itAtt = node.append_attribute(PACKET_ATTRIBUTE_ID);
                itAtt.set_value(player->getId());

                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }


            /**
            * @brief 房间解散报文
            **/
            string roomDismissPacket(const boost::shared_ptr<ServerPlayer> player){
                pugi::xml_document doc;
                pugi::xml_node node = doc.append_child(PACKET_NAME_ROOM_DESTORY);
                stringstream ss;
                node.print(ss,"\t",pugi::format_raw);
                return ss.str();
            }
        }
    }
}