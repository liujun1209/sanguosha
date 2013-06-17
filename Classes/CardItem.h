#ifndef __CARD_ITEM_H__
#define __CARD_ITEM_H__

#include <cocos2d.h>
#include <boost\smart_ptr.hpp>
#include "Card.h"
#include "CCTouchNode.h"

using namespace cocos2d;
using namespace sgs::core;

namespace sgs {
    namespace ui {

        /**
        * @brief 展示牌类
        **/
        class CardItem : public CCTouchNode
        {
        public:
            CardItem(boost::shared_ptr<Card> cardPtr);
            ~CardItem(void);
            bool init();
            static CardItem* create(boost::shared_ptr<Card> cardPtr =  boost::shared_ptr<Card>());
            void touch();

            //选中
            void select();
            //取消选中
            void unselect();
            //消失
            void fadeOut();

            void clearup();
            
            boost::shared_ptr<Card> getCardPtr();

        private:
            boost::shared_ptr<Card> cardPtr_;
            bool selected_;
        };

    }
}
#endif