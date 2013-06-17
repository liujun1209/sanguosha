
#include "TableScene.h"
#include "CardItem.h"

#include "SelectorManager.h"

using namespace sgs::selector;

namespace sgs {
    namespace ui {

        static const char* CARD_ITEM_FORMAT_PATH = "poker_%s.png";
        static const char* CARD_SUIT_FORMAT_PATH = "suit_%s.png";
        static const char* CARD_NUMBER_FORMAT_PATH = "number_%s_%d.png";

        static const char* CARD_ITEM_BACK_PATH = "poker_back.png";
        static const float FADEOUT_TIME = 0.5f;
        static const char* RED = "red";
        static const char* BLACK = "black";
        CardItem::CardItem(boost::shared_ptr<Card> cardPtr) : cardPtr_(cardPtr), selected_(false)
        {
        }


        CardItem::~CardItem(void)
        {
        }

        bool CardItem::init() {
            bool bRet = false;
            do 
            {  
                if(cardPtr_.get()) {

                    this->setTag(cardPtr_->getId());
                    //main
                    char cardPath[50] = {0};
                    sprintf(cardPath, CARD_ITEM_FORMAT_PATH ,cardPtr_->getName().c_str());
                    CCSprite* bgSprite = CCSprite::create(cardPath);
                    CC_BREAK_IF(!bgSprite);
                    bgSprite->setPosition(CCPointZero);
                    bgSprite->setAnchorPoint(CCPointZero);

                    this->addChild(bgSprite);

                    //suit
                    char cardSuitPath[50] = {0};
                    sprintf(cardSuitPath, CARD_SUIT_FORMAT_PATH ,cardPtr_->getSuitString().c_str());
                    CCSprite* suitSprite = CCSprite::create(cardSuitPath);
                    CC_BREAK_IF(!suitSprite);
                    suitSprite->setPosition(ccp(19,129));

                    this->addChild(suitSprite);

                    //number
                    char cardNumberPath[50] = {0};
                    const char* color;
                    if(cardPtr_->getColor() == Card::Red) {
                        color = RED;
                    }else {
                        color = BLACK;
                    }
                    sprintf(cardNumberPath, CARD_NUMBER_FORMAT_PATH , color , cardPtr_->getNumber());
                    CCSprite* numberSprite = CCSprite::create(cardNumberPath);
                    CC_BREAK_IF(!numberSprite);
                    numberSprite->setPosition(ccp(19, 109));

                    this->addChild(numberSprite);
                }
                else {
                    //反面
                    CCSprite* bgSprite = CCSprite::create(CARD_ITEM_BACK_PATH);
                    CC_BREAK_IF(!bgSprite);
                    bgSprite->setPosition(CCPointZero);
                    bgSprite->setAnchorPoint(CCPointZero);

                    this->addChild(bgSprite);
                }

                bRet = true;
            }while(0);
            return bRet;
        }

        boost::shared_ptr<Card> CardItem::getCardPtr() {
            return cardPtr_;
        }


        void CardItem::touch() {
            if(selected_) {
                CARD_SELECTOR.unselecteCard(this);
            }
            else {
                CARD_SELECTOR.selecteCard(this);
            }
            /*
            CardAreaBoard * cardAreaBoard = (CardAreaBoard*)this->getParent();
            cardAreaBoard->removeCard(this);
            sgs::layer::TableAnimationLayer * animationLayer = ((sgs::scene::PlayerTable*)(this->getParent()->getParent()->getParent()))->getTableAnimationLayer();
            animationLayer->playThrowAnimation(this);
            CCLog("touch(%s: %d)", cardPtr_->getSuitString().c_str(), cardPtr_->getNumber());
            */
        }

        CardItem* CardItem::create(boost::shared_ptr<Card> cardPtr) {
            CardItem* pRet = new CardItem(cardPtr);
            if(pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }

        void CardItem::fadeOut() {
            CCArray* childArray = this->getChildren();
            int count = childArray->count();
            for(int i = 0; i <count ; i++) {
                CCNode* child = (CCNode*)childArray->objectAtIndex(i);
                child->runAction(CCFadeOut::create(FADEOUT_TIME));
            }
            runAction(CCSequence::create(
                CCDelayTime::create(FADEOUT_TIME), 
                CCCallFunc::create(this,callfunc_selector(CardItem::clearup)),
                NULL));

        }

        void CardItem::clearup() {
            this->removeFromParentAndCleanup(true);
        }

        //选中
        void CardItem::select() {
            if(!selected_) {
                selected_ = true;
                setPositionY(getPositionY() + 20);
            }
        }
        
        //取消选中
        void CardItem::unselect() {
            if(selected_) {
                selected_ = false;
                setPositionY(getPositionY() - 20);
            }
        }

    }
}