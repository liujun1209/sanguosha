#ifndef __CARD_SELECTOR_H__
#define __CARD_SELECTOR_H__

#include <cocos2d.h>
#include "CardItem.h"

using namespace cocos2d;
using namespace sgs::ui;

namespace sgs {
    namespace selector {

        /**
        * 卡片选择器
        **/
        class CardSelector
        {
        public:

            CardSelector(void);
            ~CardSelector(void);

            void selecteCard(CardItem* cardPtr);
            void unselecteCard(CardItem* cardPtr);

            //设置选择数量
            void setSelectCount(const int count);
            //获取选择的卡片
            CCArray* getSelectedCards();

            //清理选择的结果，不做任何卡片相关操作，只清空选择器
            void clear();

        private:
            int selectCount_;
            CCArray* cardArray_;
        };

    }
}

#endif