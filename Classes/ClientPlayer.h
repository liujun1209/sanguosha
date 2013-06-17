#ifndef __CLIENT_PLAYER_H__
#define __CLIENT_PLAYER_H__

#include <boost\enable_shared_from_this.hpp>
#include "Player.h"
#include "Tag.h"

using namespace sgs::core;
using namespace sgs::tagParser;

namespace sgs {
    namespace ui {
        class PlayerUI;
    }
    
    using namespace sgs::ui;
    namespace client {

        class SgsClient;
        

        class ClientPlayer : public Player, public boost::enable_shared_from_this<ClientPlayer>
        {
        public:
            ClientPlayer();
            ~ClientPlayer(void);
            //重载函数
            virtual void setGeneral(boost::shared_ptr<General> general);
            virtual void setRole(Player::Role role);
            virtual void drawCards(vector<int> cardIds);
            virtual void setPhase(Player::Phase phase);
            virtual void discardCards(const set<int>& discardCards);

            //子类独有
            virtual void setPlayerUI(PlayerUI* playerUI);

            //执行报文
            void execute(xmlDocumentPtr& doc);
            
            virtual void executeDrawCards(const pugi::xml_node& node);
            virtual void executeDiscardCards(const pugi::xml_node& node);
            virtual void executeForceDiscardCards(const pugi::xml_node& node);
            virtual void executeChooseLord(const pugi::xml_node& node);
            virtual void executeChooseGeneral(const pugi::xml_node& node);
            virtual void executePhaseChange(const pugi::xml_node& node);
            virtual void executeShowTimeBar(const pugi::xml_node& node);
            virtual void executeHiddenTimeBar(const pugi::xml_node& node);
            

        protected:
            PlayerUI* playerUI_;
        };

        //
        class ControlClientPlayer : public ClientPlayer  {
        public :
            void executeDrawCards(const pugi::xml_node& node);
            void executeChooseLord(const pugi::xml_node& node);
            void executeChooseGeneral(const pugi::xml_node& node);

            virtual void drawCards(vector<int> cardIds);
            virtual void setGeneral(boost::shared_ptr<General> general);

            virtual void setSgsClient(boost::shared_ptr<SgsClient> sgsClient);
        private:
            boost::shared_ptr<SgsClient> sgsClient_;
        };

    }
}

#endif