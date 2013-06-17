#ifndef __PLAYER_CONTROLLER_BOARD_H__
#define __PLAYER_CONTROLLER_BOARD_H__

#include <cocos2d.h>
#include <boost\smart_ptr.hpp>
#include <cocos-ext.h>

#include <vector>
#include "Card.h"
#include "CCTouchNode.h"
#include "CardItem.h"
#include "PlayerUI.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace sgs::core;
using namespace sgs::client;

namespace sgs {
    namespace ui {

        /**
        * @brief 用户面板
        **/
        class GeneralInfoBoard : public CCNode  {
        public:
            GeneralInfoBoard();
            bool init();
            CREATE_FUNC(GeneralInfoBoard);
            
            void setGeneral(boost::shared_ptr<General> general); 
            void setRole(Player::Role role);
            void addBloodBar(int maxHp); 
        private:
            static const int CHILD_GENERAL_HEAD_ITEM_TAG = 1;
            static const int CHILD_BLOOD_BAR_TAG = 2;
        private:

        };

        /**
        * @brief 装备区
        **/
        class GeneralEquipBoard : public CCNode {
        public:
            GeneralEquipBoard();
            bool init();
            CREATE_FUNC(GeneralEquipBoard);
        };

        /**
        * @brief CardAreaBoard
        **/
        class CardAreaBoard : public CCTouchNode {
        public:
            enum Direction { Left, Right};
            CardAreaBoard();
            bool init();
            CREATE_FUNC(CardAreaBoard);

            /**
            * @brief 添加牌
            **/
            void addCard(vector<boost::shared_ptr<Card> > cards, Direction direction = Right);
            CCArray* getCardItemArray();
            /**
            * @brief 移除卡片
            **/
            void removeCard(CardItem * cardPtr);
            void removeCard(CCArray* cardPtr);

            void updateCardPosition();

            void touch();
        private:
            CCPoint getCardPositionByIndex(int index);
        private:
            CCArray cardsArray_;
            
            static const int POKER_WIDTH = 100;
            static const int AREA_WIDTH = 586;
        };

         /**
        * @brief 玩家控制面板
        **/
        class PlayerControlBoard : public CCNode, public PlayerUI {
        public:
            PlayerControlBoard();
            ~PlayerControlBoard();
            bool init();
            CREATE_FUNC(PlayerControlBoard);

            //抓牌
            virtual void drawCard(vector<boost::shared_ptr<Card> > cards);
            
            virtual void discardCards(set<boost::shared_ptr<Card> > cards);

            //主公选将
            void showLordSelDialog(const vector<string>& generals, boost::shared_ptr<ClientPlayer> player);
            //其他人选将
            void showOtherSelDialog(const string& lord, const vector<string>& generals, boost::shared_ptr<ClientPlayer> player);

            void setPlayer(boost::shared_ptr<ClientPlayer> player);
            void setRole(Player::Role role);
            void setPhase(Player::Phase phase);


            void okTouchDownAction(CCObject* pSender, CCControlEvent event);
            void cancelTouchDownAction(CCObject* pSender, CCControlEvent event);

            void showTimeBar(float time);
            void hiddenTimeBar();

        private:
            static const int CHILD_GENERAL_INFO_TAG = 10;
            static const int CHILD_CARD_AREA_TAG = 11;
            static const int CHILD_PHASE_ANIMATION_AREA_TAG = 12;
            static const int CHILD_TIME_BAR_TAG = 13;
            
        };

    }
}

#endif
