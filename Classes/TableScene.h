#ifndef __TABLE_SCENE_H__
#define __TABLE_SCENE_H__

#include "SgsClient.h"
#include "SgsServer.h"
#include "General.h"
#include <cocos2d.h>
#include <boost\smart_ptr.hpp>
#include "CCTouchNode.h"
#include "TableAnimationLayer.h"
#include "PlayerUI.h"

using namespace cocos2d;
using namespace sgs::core;
using namespace sgs::ui;

namespace sgs {
    namespace scene {

        

        /**
        * @brief 玩家装备区
        */
        class PlayerEquipArea : public CCNode {
        public:
        private:

        };
        

       
        
        

        /**
        * @brief 角色面板
        **/
        class RolesBoard : public CCNode  {
        public:
            RolesBoard();
            ~RolesBoard();
            bool init();
            CREATE_FUNC(RolesBoard);
            void reset(int lord, int loyalist, int rebel, int renegade);
            void dead(Player::Role role);
        private:
            void update(Player::Role role);
            int lordNum_;
            int lordMaxNum_;
            int loyalistNum_;
            int loyalistMaxNum_;
            int rebelNum_;
            int rebelMaxNum_;
            int renegadeNum_;
            int renegadeMaxNum_;

            static const int CHILD_LABEL_LORD_TAG = 0;
            static const int CHILD_LABEL_LOYALIST_TAG = 1;
            static const int CHILD_LABEL_REBEL_TAG = 2;
            static const int CHILD_LABEL_RENEGADE_TAG = 3;
        };

        /**
        * @brief 剩余牌数面板
        **/
        class LeftPilesBoard : public CCNode  {
        public:
            LeftPilesBoard();
            ~LeftPilesBoard();
            bool init();
            CREATE_FUNC(LeftPilesBoard);
            void reduce(int num);
            void setLeftPiles(int num);
        private:
            int leftPiles_;
            static const int CHILD_NUMBER_LABEL_TAG = 0;
        };

       

        /**
        * @brief 玩家桌子
        **/
        class PlayerTable :public CCLayer {
        public:
            static boost::shared_ptr<sgs::client::SgsClient> client_;
            static boost::shared_ptr<sgs::server::SgsServer> server_;
        public:
            PlayerTable(int count);
            ~PlayerTable();
            virtual bool init();
            //进入后向服务器发送准备好信息
            virtual void onEnter ();
            static PlayerTable * create(int count);

            static cocos2d::CCScene* scene(int count);
            CCPoint getPlayerBoardPosition(int index);

            //剩余牌数量
            void reduce(int num);
            void setLeftPiles(int num);
            //设置角色数量
            void setRoleInfo(int lord, int loyalist, int rebel, int renegade);

            PlayerUI* getPlayerSeat(int seat);
            
            sgs::layer::TableAnimationLayer* getTableAnimationLayer();
        private:
            int playerCount_;
            bool ready_;
            static const int CHILD_LEFT_PILES_TAG = 12;
            static const int CHILD_ROLE_BOARD_TAG = 13;
            static const int ANIMATION_LAYER_TAG = 10;
        };

    }
}

#endif