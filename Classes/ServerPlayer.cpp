#include <boost\foreach.hpp>
#include "SgsServer.h"
#include "ServerPlayer.h"
#include "PacketFactory.h"
#include "Engine.h"
#include "MyLogger.h"

using namespace sgs::utils;
using namespace sgs::packet;

namespace sgs {
    namespace server {

        static int GLOBAL_ID = 0;

        ServerPlayer::ServerPlayer(void)
        {
            Player::setId(++GLOBAL_ID);
        }


        ServerPlayer::~ServerPlayer(void)
        {
        }


         //执行
        void ServerPlayer::execute(xmlDocumentPtr& doc) {
            pugi::xml_node node = doc->first_child();
            int id = node.attribute(sgs::packet::PACKET_ATTRIBUTE_ID).as_int();
            //检查消息合法性
            if(equals(node.name(), sgs::packet::PACKET_PLAYER_MESSAGE)&& getId() == id) {
                bool broadcast = true;
                pugi::xml_node child = node.first_child();
                if(child.name()== NULL || child.name()=="") {
                    LOG_DEBUG("node name is null-===========================================");
                    return;
                }
                //选将
                if(equals(child.name(), sgs::packet::PACKET_NAME_CHOOSE_GENERAL)) {
                    LOG_DEBUG("ServerPlayer::execute PACKET_NAME_CHOOSE_GENERAL");
                    pugi::xml_attribute name = child.attribute(PACKET_ATTRIBUTE_NAME);
                    setGeneral(Engine::getInstance()->getGeneral(string(name.value())));
                    if(getRole() != Player::Lord){
                        broadcast = false;
                    }
                }
                if(broadcast) {
                    stringstream ss;
                    node.print(ss,"\t",pugi::format_raw);
                    send2OtherPacket(ss.str());
                }
            }
            else {
                CCLog("ServerPlayer::execute invalidate");
            }
        }

        bool ServerPlayer::need2Discard() {
            return getHp() < getHandCardNum();
        }

        void ServerPlayer::forceDiscard() {
            if(need2Discard()) {
                int discardNum = getHandCardNum() - getHp();
                set<int> cards;
                for(int i = 0; i < discardNum; i++) {
                    cards.insert(handCards_.at(i));
                }
                discardCards(cards);
                
                sendPacket(sgs::packet::server::discardCards(shared_from_this(), cards));
                //
            }
        }

        
        void ServerPlayer::discardCards(const set<int>& discardCards) {
            LOG_DEBUG("ServerPlayer::discardCards size= %d", discardCards.size());
            Player::discardCards(discardCards);
            if(server_) {
                    server_->discardCards(discardCards);
            }
            send2OtherPacket(sgs::packet::server::discardCards(shared_from_this(), discardCards));
        }

        void ServerPlayer::drawCards(vector<int> cardIds) {
            LOG_DEBUG("ServerPlayer::drawCards size= %d", cardIds.size());
            Player::drawCards(cardIds);
            if(server_) {
                //发送给自己
                sendPacket(sgs::packet::server::drawCardPacket(shared_from_this(), cardIds));
                //发送给其他人
                send2OtherPacket(sgs::packet::server::drawCardPacket(shared_from_this(),cardIds, false));
            }
        }

        void ServerPlayer::setGeneral(boost::shared_ptr<General> general) {
            LOG_DEBUG("ServerPlayer::setGeneral general = %s", general->getName().c_str());
            Player::setGeneral(general);
            if(server_) {
                if(getRole() == Player::Lord) {
                    server_->assignOtherGeneral(general->getName());
                }
                else {
                    server_->checkChooseGeneral();
                }
            }
        }

        void ServerPlayer::setPhase(Player::Phase phase) {
            
            Player::setPhase(phase);
            if(phase == Player::Start || phase == Player::Judge || phase == Player::Draw || phase == Player::Play || phase == Player::Discard || phase == Player::Finish) {
                LOG_DEBUG("ServerPlayer::setPhase phase = %d" , phase);
                send2AllPacket(sgs::packet::server::phaseChangePacket(shared_from_this(), phase));
            }
        }

        //发送位置和角色
        void ServerPlayer::assignSeatAndRole(int lord, int loyalist, int rebel, int renegade) {
            string packet = sgs::packet::server::seatAndRolePacket(shared_from_this(), lord, loyalist, rebel, renegade);
            sendPacket(packet);
        }

        //发送报文至客户端
        void ServerPlayer::sendPacket(const string& packet) {
            if(server_) {
                server_->sendPacket(shared_from_this(), packet);
            }
        }

        //发送报文至客户端
        void ServerPlayer::send2OtherPacket(const string& packet) {
            if(server_) {
                server_->broadcastPacketExclude(shared_from_this(), packet);
            }
        }

         //发送报文至客户端
        void ServerPlayer::send2AllPacket(const string& packet) {
            if(server_) {
                server_->broadcastPacket(packet);
            }
        }

        void ServerPlayer::setSgsServer(boost::shared_ptr<SgsServer> server) {
            server_ = server;
        }

        

        void ServerPlayer::askChooseLordGeneral(const vector<string> & generals) {
            sendPacket(sgs::packet::server::chooseLordGeneral(shared_from_this(), generals));
        }

        void ServerPlayer::askChooseGeneral(const string & lord, const vector<string> & generals) {
            sendPacket(sgs::packet::server::chooseGeneral(shared_from_this(), lord, generals));
        }

        void ServerPlayer::showTimeBar(const float& time) {
            send2AllPacket(sgs::packet::server::showTimeBarPacket(shared_from_this(), time));
        }

        void ServerPlayer::hiddenTimeBar() {
            send2AllPacket(sgs::packet::server::hiddenTimeBarPacket(shared_from_this()));
        }


    };
};