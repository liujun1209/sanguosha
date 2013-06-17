#include "ChooseGeneralLayer.h"
#include <boost\foreach.hpp>
#include <cocos-ext.h>
#include "MyLogger.h"

#include "StringResource.h"

using namespace sgs::utils;
using namespace cocos2d::extension;

namespace sgs {
    namespace layer {

        static const int LORD_GENERAL_COUNT = 6;
        static const int OTHER_GENERAL_COUNT = 6;
        //主公武将位置
        static const CCPoint GENERAL_POSITION[LORD_GENERAL_COUNT] = {
                ccp(93,333), ccp(247,333), ccp(403,333), ccp(556,333), ccp(712,333), ccp(865,333)
            };

        GeneralItem * GeneralItem::create(boost::shared_ptr<General> general, bool disable) {
            GeneralItem* pRet = new GeneralItem(general, disable);
            if(pRet && pRet->init()) {
                pRet->autorelease();
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }
        

        GeneralItem::GeneralItem(boost::shared_ptr<General> general,bool disable): general_(general), disable_(disable) {
        
        }

        bool GeneralItem::init() {
            bool pRet = false;
            do {
                //武将不能为空
                CC_BREAK_IF(!general_);

                //bg
                char pic_path[50] = {0};
                sprintf(pic_path, "general_card_%s.png", general_->getName().c_str());
                
                CCSprite* card = CCSprite::create(pic_path);
                card->setPosition(CCPointZero);
                this->addChild(card, 0, CARD_TAG);
                if(disable_) {
                    //置灰
                    card->runAction(CCTintTo::create(0,96,96,96));
                }
                pRet = true;
            }while(0);
            return pRet;
        }

        bool GeneralItem::isDisable() {
            return disable_;
        }

        void GeneralItem::touch() {
            ChooseGeneralLayer * layer = (ChooseGeneralLayer*)getParent();
            if(layer) {
                if(!disable_) {
                    CCSprite* card = (CCSprite*)this->getChildByTag(CARD_TAG);
                    CCRepeatForever * repeatAction = CCRepeatForever::create(
                        (CCActionInterval*)(CCSequence::create(
                            CCScaleBy::create(1 , 1.1f, 1.1f),
                            CCScaleBy::create(1 , 0.909f, 0.909f),
                            NULL)));
                    repeatAction->setTag(CARD_SELECT_ACTION_TAG);
                    card->runAction(repeatAction);
                }
                layer->selectGeneral(this, general_);
            }
            
        }

        void GeneralItem::unselect() {
            CCSprite* card = (CCSprite*)this->getChildByTag(CARD_TAG);
            card->stopActionByTag(CARD_SELECT_ACTION_TAG);
            card->setScale(1.0f);
        }

        bool GeneralDetailBoard::init() {
            bool bRet = false;
            do {
                //bg
                CCScale9Sprite* sprite = CCScale9Sprite::create("chatback.png");
                sprite->setPreferredSize(CCSizeMake(960, 300));
                sprite->setPosition(0, 0);
                sprite->setAnchorPoint(ccp(0,1));
                this->addChild(sprite);
                //Text
                CCLabelTTF* text = CCLabelTTF::create("", "黑体", 30, CCSize(900,300), kCCTextAlignmentLeft);
                text->setPosition(ccp(10, -20));
                text->setAnchorPoint(ccp(0,1));
                this->addChild(text, 1, TEXT_TAG);
                bRet = true;
            }while(0);
            return bRet;
        }

        void GeneralDetailBoard::setString(const string& msg) {
            CCLabelTTF* text = (CCLabelTTF*)getChildByTag(TEXT_TAG);
            text->setString(msg.c_str());
        }

        ChooseGeneralLayer::ChooseGeneralLayer(void):currentGeneralItem_(NULL)
        {
        }


        ChooseGeneralLayer::~ChooseGeneralLayer(void)
        {
        }

        bool ChooseGeneralLayer::init(const char* title) {
            bool pRet = false;
            do {
                CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(0, 0 ,0,155)));

                CCSprite* bg = CCSprite::create("system_sel_general_bg.png");
                bg->setPosition(ccp(480,320));
                this->addChild(bg);

                CCLabelBMFont* titleFont = CCLabelBMFont::create(title, "bmfont_select_general.fnt");
                titleFont->setPosition(ccp(480, 458));
                this->addChild(titleFont);

                pRet = true;
            }while(0);
            return pRet;
        }

         void ChooseGeneralLayer::onEnter() {
            CCLayerColor::onEnter();
            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
        }

        void ChooseGeneralLayer::onExit() {
            CCLayerColor::onExit();
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        }

        bool ChooseGeneralLayer::ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent){
           return false;
        }

        void ChooseGeneralLayer::closeChoseLayer() {
            this->removeFromParentAndCleanup(true);
        }
        
        void ChooseGeneralLayer::selectGeneral(GeneralItem* generalItem, boost::shared_ptr<General> generalPtr) {
            if(generalPtr_ != generalPtr) {
                if(currentGeneralItem_){
                    currentGeneralItem_->unselect();
                }
                currentGeneralItem_ = generalItem;
                //不同则换武将
                showGeneralDetail(generalPtr);
            }
            else {
                //相同 选中并退出
                if(!generalItem->isDisable()){
                    if(callbackFunc_) {
                        callbackFunc_(generalPtr);
                    }
                    this->closeChoseLayer();
                }
            }
        }

        void ChooseGeneralLayer::setSelectCallback(boost::function<void(boost::shared_ptr<General>)> callback) {
            callbackFunc_ = callback;
        }

        void ChooseGeneralLayer::showGeneralDetail(boost::shared_ptr<General> generalPtr) {
            if(!generalPtr_) {
                //构造显示区域
                GeneralDetailBoard* board = GeneralDetailBoard::create();
                board->setAnchorPoint(ccp(0,1));
                board->setPosition(ccp(0,640));
                this->addChild(board , 1, DETAIL_BOARD_TAG);
            }
            
            if(generalPtr_ != generalPtr) {
                generalPtr_ = generalPtr;
                GeneralDetailBoard* board = (GeneralDetailBoard*)this->getChildByTag(DETAIL_BOARD_TAG);
                board->setString(generalPtr_->getSkillDescription());
            }
        }

        ChooseLordLayer * ChooseLordLayer::create(vector<boost::shared_ptr<General> > generals) {
            ChooseLordLayer* pRet = new ChooseLordLayer();
            if(pRet && pRet->init(generals)) {
                pRet->autorelease();
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }

        bool ChooseLordLayer::init(vector<boost::shared_ptr<General> > generals) {
            bool pRet = false;
            do {
               //string lordString = STRING_RESOURCE("lordChooseGeneral");
                //LOG_DEBUG("lordChoose %s", lordString.c_str());
                CC_BREAK_IF(!ChooseGeneralLayer::init(STRING_RESOURCE("lordChooseGeneral").c_str()));

                int index = 0;
                BOOST_FOREACH(boost::shared_ptr<General> general, generals) {
                    GeneralItem * generalItem = GeneralItem::create(general);
                    generalItem->setPosition(GENERAL_POSITION[index++]);
                    this->addChild(generalItem);
                    if(index > LORD_GENERAL_COUNT - 1) {
                        break;
                    }
                }
                
                pRet = true;
            }while(0);
            return pRet;
        }

        ChooseOtherLayer * ChooseOtherLayer::create(boost::shared_ptr<General> lord, vector<boost::shared_ptr<General> > generals) {
            ChooseOtherLayer* pRet = new ChooseOtherLayer();
            if(pRet && pRet->init(lord, generals)) {
                pRet->autorelease();
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }


        bool ChooseOtherLayer::init(boost::shared_ptr<General> lord, vector<boost::shared_ptr<General> > generals) {
            bool pRet = false;
            do {
                //LOG_DEBUG("chooseGeneral %s", generalString.c_str());
                CC_BREAK_IF(!ChooseGeneralLayer::init(STRING_RESOURCE("chooseGeneral").c_str()));
                //lord
                GeneralItem * generalItem = GeneralItem::create(lord, true);
                generalItem->setPosition(GENERAL_POSITION[0]);
                this->addChild(generalItem);
                //主公武将
                //string lordGeneralString = STRING_RESOURCE("lordGeneral");
                //LOG_DEBUG("%s", lordGeneralString.c_str());
                CCLabelBMFont* generalFont = CCLabelBMFont::create(STRING_RESOURCE("lordGeneral").c_str(), "bmfont_select_general.fnt");
                generalFont->setPosition(ccp(93, 200));
                this->addChild(generalFont);

                //generals
                int index = 1;
                BOOST_FOREACH(boost::shared_ptr<General> general, generals) {
                    GeneralItem * generalItem = GeneralItem::create(general);
                    generalItem->setPosition(GENERAL_POSITION[index++]);
                    this->addChild(generalItem);
                    if(index > OTHER_GENERAL_COUNT - 1) {
                        break;
                    }
                }

                pRet = true;
            }while(0);
            return pRet;
        
        }
    }
}