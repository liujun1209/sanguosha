#include "TableScene.h"
#include "SgsClient.h"
#include "ClientPlayer.h"
#include "PlayerUI.h"
#include "UIThreadMQ.h"
#include "PacketFactory.h"
#include "PlayerControllerBoard.h"
#include "Engine.h"
#include "SgsUtil.h"
#include "MyLogger.h"

using namespace sgs::utils;
using namespace sgs::core;
using namespace sgs::packet;

namespace sgs {
    namespace client {

        ClientPlayer::ClientPlayer(): playerUI_(NULL)
        {
        }


        ClientPlayer::~ClientPlayer(void)
        {
        }

        void ClientPlayer::setPlayerUI(PlayerUI* playerUI) {
            playerUI_ = playerUI;
        }

        void ClientPlayer::setGeneral(boost::shared_ptr<General> general) {
            Player::setGeneral(general);
            if(playerUI_) {
                ADD_MESSAGE(boost::bind(&PlayerUI::setPlayer, playerUI_, shared_from_this()));
            }
        }

        void ClientPlayer::setRole(Player::Role role) {
            Player::setRole(role);
            if(playerUI_) {
                ADD_MESSAGE(boost::bind(&PlayerUI::setRole, playerUI_, role));
            }
        }

        void ClientPlayer::setPhase(Player::Phase phase) {
            Player::setPhase(phase);
            if(playerUI_) {
                ADD_MESSAGE(boost::bind(&PlayerUI::setPhase, playerUI_, phase));
            }
        }

        //执行
        void ClientPlayer::execute(xmlDocumentPtr& doc) {
            pugi::xml_node node = doc->first_child();
            int id = node.attribute(sgs::packet::PACKET_ATTRIBUTE_ID).as_int();
            if(node.name()== NULL || node.name()=="") {
                CCLog("node name is null-===========================================");
                return;
            }
            //检查消息合法性
            if(equals(node.name(), sgs::packet::PACKET_PLAYER_MESSAGE)&& getId() == id) {
                pugi::xml_node child = node.first_child();
                //选主公
                if(equals(child.name(), sgs::packet::PACKET_NAME_CHOOSE_LORD)) {
                    executeChooseLord(child);
                    return;
                }
                //其他人选将
                if(equals(child.name(), sgs::packet::PACKET_NAME_CHOOSE_GENERAL)) {
                    executeChooseGeneral(child);
                    return;
                }
                //抓牌
                if(equals(child.name(), sgs::packet::PACKET_NAME_DRAW_CARD)) {
                    executeDrawCards(child);
                    return;
                }
                //阶段改变
                if(equals(child.name(), sgs::packet::PACKET_NAME_PHASE_CHANGE)) {
                    executePhaseChange(child);
                    return;
                }
                //显示时间条
                if(equals(child.name(), sgs::packet::PACKET_NAME_SHOW_TIME_BAR)) {
                    executeShowTimeBar(child);
                    return;
                }
                //隐藏时间条
                if(equals(child.name(), sgs::packet::PACKET_NAME_HIDDEN_TIME_BAR)) {
                    executeHiddenTimeBar(child);
                    return;
                }
                //弃牌
                if(equals(child.name(), sgs::packet::PACKET_NAME_DISCARD_CARD)) {
                    executeDiscardCards(child);
                    return;
                }
                //强制弃牌
                if(equals(child.name(), sgs::packet::PACKET_NAME_FORCE_DISCARD_CARD)) {
                    executeForceDiscardCards(child);
                    return;
                }
            }
            else {
                LOG_DEBUG("Clientplayer::execute invalidate %d ,%d", getId(), id);

            }
        }

        void ClientPlayer::executeForceDiscardCards(const pugi::xml_node& node) {
            LOG_DEBUG("ClientPlayer::executeForceDiscardCards");
            //子类实现
        }

        void ClientPlayer::executeDiscardCards(const pugi::xml_node& node) {
            LOG_DEBUG("ClientPlayer::executeDiscardCards");
            set<int> cardIds;
            pugi::xml_node cardNode = node.child(sgs::packet::PACKET_NAME_CARD);
            while(cardNode) {
                pugi::xml_attribute idAtt = cardNode.attribute(sgs::packet::PACKET_ATTRIBUTE_ID);
                cardIds.insert(idAtt.as_int());
                cardNode = cardNode.next_sibling();
            }
            
            discardCards(cardIds);
        }

        void ClientPlayer::executeHiddenTimeBar(const pugi::xml_node& node) {
            LOG_DEBUG("ClientPlayer::executeHiddenTimeBar");
            if(playerUI_) {
                ADD_MESSAGE(boost::bind(&PlayerUI::hiddenTimeBar, playerUI_));
            }
        }

        void ClientPlayer::executeShowTimeBar(const pugi::xml_node& node) {
            LOG_DEBUG("ClientPlayer::executeShowTimeBar");
            pugi::xml_attribute timeAtt = node.attribute(PACKET_ATTRIBUTE_TIME);
            float time = timeAtt.as_float();
            if(playerUI_) {
                ADD_MESSAGE(boost::bind(&PlayerUI::showTimeBar, playerUI_, time));
            }
        }

        void ClientPlayer::executePhaseChange(const pugi::xml_node& node) {
            LOG_DEBUG("ClientPlayer::executePhaseChange");
            pugi::xml_attribute phase = node.attribute(PACKET_ATTRIBUTE_PHASE);
            setPhase(sgs::utils::getPhaseFromString(string(phase.value())));
        }


        void ClientPlayer::executeChooseLord(const pugi::xml_node& node) {
            //子类实现
        }

        void ClientPlayer::executeChooseGeneral(const pugi::xml_node& node) {
            LOG_DEBUG("ClientPlayer::executeChooseGeneral");
            pugi::xml_attribute name = node.attribute(PACKET_ATTRIBUTE_NAME);
            setGeneral(Engine::getInstance()->getGeneral(string(name.value())));
        }

        void ClientPlayer::executeDrawCards(const pugi::xml_node& node) {
            LOG_DEBUG("ClientPlayer::executeDrawCards");
            pugi::xml_attribute numAtt = node.attribute(PACKET_ATTRIBUTE_NUM);
            int num = numAtt.as_int();
            //addNum();
            vector<int> cardIds;
            for(int i = 0; i < num ; i++ ) {
                cardIds.push_back(-1);
            }
            drawCards(cardIds);
        }
        
        void ClientPlayer::drawCards(vector<int> cardIds) {
            LOG_DEBUG("ClientPlayer::discardCards size= %d", cardIds.size());
            if(playerUI_) {
                 ADD_MESSAGE(boost::bind(&PlayerUI::drawCard,playerUI_,Engine::getInstance()->getCards(cardIds)));
            }
        }

        void ClientPlayer::discardCards(const set<int>& discardCards) {
            LOG_DEBUG("ClientPlayer::discardCards size= %d", discardCards.size());
            if(playerUI_) {
                 ADD_MESSAGE(boost::bind(&PlayerUI::discardCards, playerUI_, Engine::getInstance()->getCards(discardCards)));
            }
        }


        void ControlClientPlayer::setGeneral(boost::shared_ptr<General> general) {
            LOG_DEBUG("ControlClientPlayer::setGeneral general = %s" , general->getName().c_str());
            ClientPlayer::setGeneral(general);
            if(sgsClient_) {
                //发送数据给服务器
                sgsClient_->sendPacket(sgs::packet::client::selectGeneralPacket(shared_from_this(), general));
            }
        }

        void ControlClientPlayer::setSgsClient(boost::shared_ptr<SgsClient> sgsClient) {
            sgsClient_ = sgsClient;
        }

        void ControlClientPlayer::drawCards(vector<int> cardIds) {
            LOG_DEBUG("ControlClientPlayer::drawCards size = %d", cardIds.size());
            Player::drawCards(cardIds);
            ClientPlayer::drawCards(cardIds);
        }


        void ControlClientPlayer::executeDrawCards(const pugi::xml_node& node) {
            LOG_DEBUG("ControlClientPlayer::executeDrawCards");
            vector<int> cardIds;
            pugi::xml_node cardNode = node.child(sgs::packet::PACKET_NAME_CARD);
            while(cardNode) {
                pugi::xml_attribute idAtt = cardNode.attribute(sgs::packet::PACKET_ATTRIBUTE_ID);
                cardIds.push_back(idAtt.as_int());
                cardNode = cardNode.next_sibling();
            }
            drawCards(cardIds);
        }

        void ControlClientPlayer::executeChooseLord(const pugi::xml_node& node) {
            LOG_DEBUG("ControlClientPlayer::executeChooseLord");
            vector<string> generals;
            pugi::xml_node generalNode = node.child(sgs::packet::PACKET_NAME_GENERAL);

            while(generalNode) {

                pugi::xml_attribute name = generalNode.attribute(sgs::packet::PACKET_ATTRIBUTE_NAME);
                generals.push_back(string(name.value()));
            
                generalNode = generalNode.next_sibling();
            }
            PlayerControlBoard* playerControlBoard = dynamic_cast<PlayerControlBoard*>(playerUI_);
            if(playerControlBoard) {
                ADD_MESSAGE(boost::bind(&PlayerControlBoard::showLordSelDialog,playerControlBoard, generals, shared_from_this()));
            }
            else {
                LOG_DEBUG("ControlClientPlayer::executeChooseLord PlayerControlBoard is null");
            }
        }

        void ControlClientPlayer::executeChooseGeneral(const pugi::xml_node& node) {
            LOG_DEBUG("ControlClientPlayer::executeChooseGeneral");
            vector<string> generals;
            pugi::xml_node generalNode = node.child(sgs::packet::PACKET_NAME_GENERAL);

            while(generalNode) {

                pugi::xml_attribute name = generalNode.attribute(sgs::packet::PACKET_ATTRIBUTE_NAME);
                generals.push_back(string(name.value()));
            
                generalNode = generalNode.next_sibling();
            }

            pugi::xml_node lordNode = node.child(sgs::packet::PACKET_NAME_LORD);
            pugi::xml_attribute lordName = lordNode.attribute(sgs::packet::PACKET_ATTRIBUTE_NAME);

            string lord(lordName.value());
            PlayerControlBoard* playerControlBoard = dynamic_cast<PlayerControlBoard*>(playerUI_);
            if(playerControlBoard) {
                ADD_MESSAGE(boost::bind(&PlayerControlBoard::showOtherSelDialog,playerControlBoard,lord, generals, shared_from_this()));
            }
            else {
                LOG_DEBUG("ControlClientPlayer::executeChooseGeneral PlayerControlBoard is null");
            }
        }

    }
}