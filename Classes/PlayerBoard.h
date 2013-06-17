#ifndef __CC__PLAYER__BOARD_H__
#define __CC__PLAYER__BOARD_H__

#include <cocos2d.h>
#include <boost\smart_ptr.hpp>
#include <string>

#include "CCTouchNode.h"
#include "General.h"
#include "PlayerUI.h"

using namespace cocos2d;
using namespace sgs::core;

namespace sgs {
    namespace ui {

        /**
        * @brief 武将头像
        **/
       class GeneralHeadItem : public CCTouchNode {
       public:
           GeneralHeadItem(int width);
           bool init();
           static GeneralHeadItem* create(int width = IMAGE_WIDTH);

           //设置武将
           void setGeneral(boost::shared_ptr<General> general);
           //设置角色
           void setRole(Player::Role role);
       private:
           int width_;
           boost::shared_ptr<General> general_;
           static const int IMAGE_WIDTH = 135;
           static const int IMAGE_HEIGHT = 85;
           static const int ROLE_TAG = 1;
           
       };

       class Blood : public CCNode {
        public:
            Blood();
            ~Blood();
            bool init();
            CREATE_FUNC(Blood);

            void setLevel(int level);
            void damage();
            void recover();
        private:
            int level_;
        };

        enum BloodDirection { Horizontal, Vertical };
        /**
        * @brief 血条
        **/
        class BloodBar : public CCNode {
        public:
            
            BloodBar(int currentBlood, int maxBlood_, BloodDirection direction);
            ~BloodBar();
            bool init();
            static BloodBar* create(int currentBlood, int maxBlood_, BloodDirection direction);

            void damage(int value);
            void recover(int value);
            void setLevel();

            CC_PROPERTY(int , currentBlood_, CurrentBlood);
            CC_PROPERTY(int , maxBlood_, MaxBlood);
            CC_SYNTHESIZE(BloodDirection , direction_, Direction);
        };

        /**
        * @brief 玩家面板
        **/
        class PlayerBoard : public CCTouchNode, public PlayerUI  {
        public:
            PlayerBoard();
            ~PlayerBoard();
            bool init();
            CREATE_FUNC(PlayerBoard);

            /**
            * @Override
            **/
            void drawCard(vector<boost::shared_ptr<Card> > cards);
            void discardCards(set<boost::shared_ptr<Card> > cards);
            void setPlayer(boost::shared_ptr<ClientPlayer> player);
            void setRole(Player::Role role);
            void setPhase(Player::Phase phase);

            //触摸相关
            void touch();
            void longTouch();
            //
            void showTimeBar(float time);
            void hiddenTimeBar();

            void playDamageAnimation(int value);
            void playRecoverAnimation(int value);
            void playBloodAnimation(const char * text, ccColor3B color);
            void bloodAnimationCallback();
        private:
            // 点击开始位置
            //CCPoint m_beginPos;
        private:
            static const int BLOOD_BAR_TAG = 1;
            static const int ANIMATION_FONT_TAG = 2;
            static const int CHILD_GENERAL_HEAD_ITEM_TAG = 3;
            static const int CHILD_PHASE_ANIMATION_AREA_TAG = 4;
            static const int CHILD_TIME_BAR_TAG = 5;
            
        };

    }
}

#endif