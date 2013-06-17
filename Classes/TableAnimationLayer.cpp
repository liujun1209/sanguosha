#include "TableAnimationLayer.h"
#include "SgsUtil.h"
#include "CardItem.h"

using namespace sgs::utils;
using namespace sgs::ui;

namespace sgs {
    namespace layer {

        static const char* ROLE_FORMAT_PATH = "role_%s.png";
        static const float ANIMATION_MOVE_TIME = 0.5f;
        static const int AREA_WIDTH = 500;
        static const int POKER_WIDTH = 100;
        static const int DEFAULT_OFFSET = 460;
        static const int MAX_POKER_COUTN = 8;

        TableAnimationLayer::TableAnimationLayer(void)
        {
        }


        TableAnimationLayer::~TableAnimationLayer(void)
        {
        }

        bool TableAnimationLayer::init() {
            bool bRet = false;
            do {
                CC_BREAK_IF(! CCLayer::init());


                bRet = true;
            }while(0);
            return bRet;
        }

        void TableAnimationLayer::playRoleAnimation(Player::Role role) {
            char rolePath[50] = {0}; 
            string roleStr = getRoleString(role);
            sprintf(rolePath, ROLE_FORMAT_PATH ,roleStr.c_str());

            CCSprite* sprite = CCSprite::create(rolePath);
            sprite->setPosition(ccp(480, 400));

            this->addChild(sprite);
            float time = 2.0f;
            sprite->runAction(CCSequence::create(CCScaleBy::create(time, 1.5f,1.5f),
                CCSpawn::create(CCScaleTo::create(time,0.1f,0.1f),CCMoveTo::create(time,ccp(797.0f,142.0f)),NULL), 
                CCFadeOut::create(0.3f),
                CCCallFuncO::create(this, callfuncO_selector(TableAnimationLayer::remove),sprite),
                NULL));
        }

        void TableAnimationLayer::playDrawCardAnimation(CCPoint point, int count) {

            int offset  = 0;
            if(count >0) {
                offset = count /2 * 20;
            }
            float time = 0.6f;

            for(int i = 0; i< count ;i++ ) {
                CardItem* cardItem = CardItem::create();
                cardItem->setPosition(ccp(440 - offset + i*20, 320));
                this->addChild(cardItem);

                point.x = point.x + i*20;

                cardItem->runAction(CCSequence::create(
                    CCMoveTo::create(time, point),
                    CCCallFunc::create(cardItem, callfunc_selector(CardItem::fadeOut))
                    ,NULL));
            }

        }

        void TableAnimationLayer::remove(CCObject* object) {
            CCNode * node = (CCNode*)object;
            node->removeFromParentAndCleanup(true);
        }

        
        void TableAnimationLayer:: playThrowAnimation(CCArray* cardArray) {
            int size = cardItemArray_.count();
            int count = cardArray->count();
            if(size + count >= MAX_POKER_COUTN) {
                clear();
            }
            for(int i = 0; i< count ; i++) {
                CardItem * card = (CardItem *)cardArray->objectAtIndex(i);
                CCPoint point = card->convertToWorldSpace(ccp(0,0));
                card->removeFromParentAndCleanup(false);
                card->setPosition(point);

                this->addChild(card);
                cardItemArray_.addObject(card);
            }

            updateCardPosition();
        }

        void TableAnimationLayer::playThrowAnimation(CardItem * card) {
            CCArray cardArray;
            cardArray.addObject(card);
            playThrowAnimation(&cardArray);
            
        }

        CCPoint TableAnimationLayer::getPositionByIndex(const int index) {
            int size = cardItemArray_.count();
            int mid = size/2;
            if(index < size && size > 0) {
                int width = (AREA_WIDTH - POKER_WIDTH) / size;
                width = width > POKER_WIDTH ? POKER_WIDTH : width;
                int offset = DEFAULT_OFFSET + (size/2.0f-1)* width;
                return ccp(offset - (index)* width , 280);
            } 
            return CCPointZero;
        }

        //调整card位置
        void TableAnimationLayer::updateCardPosition() {
            int count = cardItemArray_.count();

            for(int i = 0; i < count ; i++ ) {
                CCPoint moveToPoint = getPositionByIndex(i);
                CardItem* cardItem = (CardItem*)cardItemArray_.objectAtIndex(i);

                cardItem->runAction(CCMoveTo::create(ANIMATION_MOVE_TIME, moveToPoint));
            }

        }

        //清理桌面
        void TableAnimationLayer::clear() {
            int size = cardItemArray_.count();
            for(int i = 0 ;i < size; i++) {
                CardItem* cardItem = (CardItem*)cardItemArray_.objectAtIndex(i);
                cardItem->fadeOut();
            }
            cardItemArray_.removeAllObjects();
        }

    }
}