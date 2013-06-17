#ifndef __TABLE_ANIMATION_LAYER_H__
#define __TABLE_ANIMATION_LAYER_H__

#include <cocos2d.h>
#include "Player.h"
#include "CardItem.h"

using namespace cocos2d;
using namespace sgs::core;
using namespace sgs::ui;

namespace sgs {
    namespace layer {

        class TableAnimationLayer : public CCLayer
        {
        public:
            TableAnimationLayer(void);
            ~TableAnimationLayer(void);
            bool init();
            CREATE_FUNC(TableAnimationLayer);

            //player
            void playRoleAnimation(Player::Role role);

            //摸牌动画
            void playDrawCardAnimation(CCPoint point, int count = 2);

            //出牌动画
            void playThrowAnimation(CardItem * card);
            void playThrowAnimation(CCArray* cardArray);

            //删除动画组件
            void remove(CCObject* object);

            //
            CCPoint getPositionByIndex(const int index);
            //调整card位置
            void updateCardPosition();

            //清理桌面
            void clear();
        private:
            CCArray cardItemArray_;

        };

    }
}
#endif