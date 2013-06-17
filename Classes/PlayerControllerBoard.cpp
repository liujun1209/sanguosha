#include <boost\foreach.hpp>

#include "TableScene.h"
#include "PlayerControllerBoard.h"

#include "PlayerBoard.h"
#include "Engine.h"
#include "CardItem.h"
#include "SelectorManager.h"

#include "ChooseGeneralLayer.h"

using namespace sgs::core;
using namespace sgs::scene;
using namespace sgs::selector;
using namespace sgs::layer;

namespace sgs {
    namespace ui {

        static const char * GENERAL_INFO_BG_PATH = "table_user.png";
        static const char * GENERAL_EQUIP_BG_PATH = "table_equ.png";
        static const char * OK_BUTTON_PATH = "button_common_button_start_table.png";

        GeneralInfoBoard::GeneralInfoBoard() {
        }

        bool GeneralInfoBoard::init() {
            bool bRet = false;
            do {
                CCSprite* userSprite = CCSprite::create(GENERAL_INFO_BG_PATH);
                CC_BREAK_IF(! userSprite);

                userSprite->setPosition(ccp(0,0));
                userSprite->setAnchorPoint(CCPointZero);
                this->addChild(userSprite);

                GeneralHeadItem* generalHeadItem = GeneralHeadItem::create();

                generalHeadItem->setRole(Player::Unknow);
                generalHeadItem->setPosition(ccp(13, 68));
                generalHeadItem->setAnchorPoint(ccp(0,0));
                this->addChild(generalHeadItem, 0 , CHILD_GENERAL_HEAD_ITEM_TAG);




                bRet = true;
            }while(0);
            return bRet;
        }


        void GeneralInfoBoard::setGeneral(boost::shared_ptr<General> general) {

            GeneralHeadItem* generalHeadItem = ( GeneralHeadItem* )this->getChildByTag(CHILD_GENERAL_HEAD_ITEM_TAG);

            generalHeadItem->setGeneral(general);
        }

        void GeneralInfoBoard::setRole(Player::Role role){
            GeneralHeadItem* generalHeadItem = ( GeneralHeadItem* )this->getChildByTag(CHILD_GENERAL_HEAD_ITEM_TAG);
            generalHeadItem->setRole(role);
        }

        void GeneralInfoBoard::addBloodBar(int maxHp) {
            BloodBar*  bloodBar = ( BloodBar*)this->getChildByTag(CHILD_BLOOD_BAR_TAG);
            if(!bloodBar) {
                bloodBar = BloodBar::create(maxHp, maxHp, Vertical);
                bloodBar->setPosition(ccp(160,136));
                bloodBar->setAnchorPoint(ccp(1,1));
                bloodBar->setScale(0.9f);
                addChild(bloodBar, 0, CHILD_BLOOD_BAR_TAG);
            }
        }

        GeneralEquipBoard::GeneralEquipBoard() {
        }

        bool GeneralEquipBoard::init() {
            bool bRet = false;
            do {
                CCSprite* bgSprite = CCSprite::create(GENERAL_EQUIP_BG_PATH);
                CC_BREAK_IF(! bgSprite);

                bgSprite->setPosition(ccp(0,0));
                bgSprite->setAnchorPoint(CCPointZero);
                this->addChild(bgSprite);
                bRet = true;
            }while(0);
            return bRet;
        }

        CardAreaBoard::CardAreaBoard() {
        }

        bool CardAreaBoard::init() {
            bool bRet = false;
            do {

                bRet = true;
            }while(0);
            return bRet;
        }

        /**
        * @brief 添加牌
        **/
        void CardAreaBoard::addCard(vector<boost::shared_ptr<Card> > cards, Direction direction) {
            CCPoint startPoint(0, 0);
            if(direction == Right) {
                startPoint.x = AREA_WIDTH;
            }
            BOOST_FOREACH(boost::shared_ptr<Card> card, cards) {
                CardItem* cardItem = CardItem::create(card);
                cardItem->setPosition(startPoint);
                this->addChild(cardItem);
                cardsArray_.addObject(cardItem);
            }

            updateCardPosition();

        }

        void CardAreaBoard::updateCardPosition() {
            int count = cardsArray_.count();
            for(int i = 0; i < count ; i++) {
                CardItem* cardItem = (CardItem*)cardsArray_.objectAtIndex(i);
                if(cardItem) {
                    CCPoint position = cardItem->getPosition();
                    CCPoint point = getCardPositionByIndex(i);
                    point.y = cardItem->getPositionY();
                    //if(point.x != position.x || point.y != position.y) {
                    cardItem->runAction(CCMoveTo::create(0.8f, point));
                    //}
                }
            }
        }

        CCArray* CardAreaBoard::getCardItemArray() {
            return &cardsArray_;
        }

        void CardAreaBoard::removeCard(CardItem * cardPtr) {
            CCArray removeCardArray;
            removeCardArray.addObject(cardPtr);
            removeCard(&removeCardArray);
        }

        void CardAreaBoard::removeCard(CCArray* removeCardArray) {
            int count = removeCardArray->count();
            for(int i = 0; i < count ; i++) {
                CardItem* cardItem = (CardItem*)removeCardArray->objectAtIndex(i);
                cardsArray_.removeObject(cardItem);
            }

            updateCardPosition();
        }

        CCPoint CardAreaBoard::getCardPositionByIndex(int index) {
            int size = cardsArray_.count();
            if(index < size && size > 0) {
                int width = (AREA_WIDTH - POKER_WIDTH) / size;
                width = width > POKER_WIDTH ? POKER_WIDTH : width;
                return ccp(index * width, 0);
            }
            return CCPointZero;
        }

        static int temp_card_count = 0;

        void CardAreaBoard::touch() {
            vector<boost::shared_ptr<Card> > cards;

            Engine * engin = sgs::core::Engine::getInstance();
            cards.push_back(engin->getCard(temp_card_count++));
            cards.push_back(engin->getCard(temp_card_count++));
            //cards.push_back(engin->getCard(temp_card_count++));
            //cards.push_back(engin->getCard(temp_card_count++));

            addCard(cards, Right);
        }

        PlayerControlBoard::PlayerControlBoard() {
        }

        PlayerControlBoard::~PlayerControlBoard() {
        }

        bool PlayerControlBoard::init() {
            bool bRet = false;
            do {
                //装备区
                GeneralEquipBoard* generalEquipBoard = GeneralEquipBoard::create();
                CC_BREAK_IF(! generalEquipBoard);

                generalEquipBoard->setPosition(CCPointZero);
                generalEquipBoard->setAnchorPoint(CCPointZero);
                this->addChild(generalEquipBoard);

                //牌区
                CardAreaBoard* cardAreaBoard = CardAreaBoard::create();
                cardAreaBoard->setPosition(ccp(188, 0));
                this->addChild(cardAreaBoard, 0, CHILD_CARD_AREA_TAG);

                //用户区

                GeneralInfoBoard* generalInfoBoard = GeneralInfoBoard::create();
                generalInfoBoard->setPosition(ccp(773,0));
                generalInfoBoard->setAnchorPoint(ccp(0,0));
                this->addChild(generalInfoBoard, 0, CHILD_GENERAL_INFO_TAG);

                //时间条
                PlayerTimeBar * timeBar =  PlayerTimeBar::create();
                timeBar->setPosition(ccp(185, 170));
                timeBar->setScaleX(2.5f);
                timeBar->setScaleY(0.7f);
                this->addChild(timeBar, 0 ,CHILD_TIME_BAR_TAG);

                //阶段动画
                PhaseAnimationArea* phaseAnimationArea = PhaseAnimationArea::create();
                phaseAnimationArea->setPosition(ccp(180, 170));
                phaseAnimationArea->setAnchorPoint(ccp(0,0));
                this->addChild(phaseAnimationArea, 3, CHILD_PHASE_ANIMATION_AREA_TAG);

                //按钮
                CCScale9Sprite* menuSprite = CCScale9Sprite::create(OK_BUTTON_PATH);  
                CCControlButton* menuBtn = CCControlButton::create(menuSprite);  
                menuBtn->setPosition(ccp(320, 200));
                menuBtn->setPreferredSize(CCSizeMake(168, 80));
                menuBtn->setAnchorPoint(ccp(0, 0));
                this->addChild(menuBtn);
                menuBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(PlayerControlBoard::okTouchDownAction), CCControlEventTouchUpInside);  


                CCScale9Sprite* menuSprite2 = CCScale9Sprite::create(OK_BUTTON_PATH);  
                CCControlButton* menuBtn2 = CCControlButton::create(menuSprite2);  
                menuBtn2->setPosition(ccp(520, 200));
                menuBtn2->setPreferredSize(CCSizeMake(168, 80));
                menuBtn2->setAnchorPoint(ccp(0, 0));
                this->addChild(menuBtn2);

                menuBtn2->addTargetWithActionForControlEvents(this, cccontrol_selector(PlayerControlBoard::cancelTouchDownAction), CCControlEventTouchUpInside);  


                bRet = true;
            }while(0);
            return bRet;
        }

        void PlayerControlBoard::showTimeBar(float time) {
            PlayerTimeBar* timeBar = (PlayerTimeBar*)getChildByTag(CHILD_TIME_BAR_TAG);PlayerTimeBar::create();
            if(timeBar) {
                timeBar->showAnimation(time);
            }
        }

        void PlayerControlBoard::hiddenTimeBar() {
            PlayerTimeBar* timeBar = (PlayerTimeBar*)getChildByTag(CHILD_TIME_BAR_TAG);PlayerTimeBar::create();
            if(timeBar) {
                timeBar->hidden();
            }

        }

        void PlayerControlBoard::discardCards(set<boost::shared_ptr<Card> > cards) {
            PlayerTable* playerTable = ((PlayerTable*)getParent());
            if(playerTable) {
                CardAreaBoard* cardAreaBoard = (CardAreaBoard*)getChildByTag(CHILD_CARD_AREA_TAG);
                if(cardAreaBoard) {
                    CCArray* handCardArray = cardAreaBoard->getCardItemArray();
                    CCArray itemArray;
                    int count = handCardArray->count();
                    for(int i = count - 1; i>=0; i--) {
                        CardItem* item = (CardItem*)handCardArray->objectAtIndex(i);
                        if(cards.find(item->getCardPtr()) != cards.end()) {
                            itemArray.addObject(item);
                        }
                    }
                    cardAreaBoard->removeCard(&itemArray);

                    sgs::layer::TableAnimationLayer* animationLayer = playerTable->getTableAnimationLayer();
                    animationLayer->playThrowAnimation(&itemArray);
                }
            }
        }

        void PlayerControlBoard::drawCard(vector<boost::shared_ptr<Card> > cards) {
            PlayerTable* playerTable = ((PlayerTable*)getParent());
            if(playerTable) {
                int size = cards.size();
                playerTable->reduce(size);
            }
            CardAreaBoard*  cardAreaBoard = (CardAreaBoard* )getChildByTag(CHILD_CARD_AREA_TAG);
            cardAreaBoard->addCard(cards);
        }

        //主公选将
        void PlayerControlBoard::showLordSelDialog(const vector<string>& generals, boost::shared_ptr<ClientPlayer> player) {
            Engine * engin = sgs::core::Engine::getInstance();

            ChooseLordLayer * chooseLayer = ChooseLordLayer::create(engin->getGeneral(generals));
            this->getParent()->addChild(chooseLayer, 99);
            chooseLayer->setSelectCallback(boost::bind(&ClientPlayer::setGeneral, player, _1));
        }

        //非主公选将
        void PlayerControlBoard::showOtherSelDialog(const string& lord, const vector<string>& generals, boost::shared_ptr<ClientPlayer> player) {
            Engine * engin = sgs::core::Engine::getInstance();

            ChooseOtherLayer * chooseLayer = ChooseOtherLayer::create(engin->getGeneral(lord),engin->getGeneral(generals));
            this->getParent()->addChild(chooseLayer, 99);
            chooseLayer->setSelectCallback( boost::bind(&ClientPlayer::setGeneral, player, _1));
        }

        void PlayerControlBoard::setPlayer(boost::shared_ptr<ClientPlayer> player) {
            PlayerUI::setPlayer(player);
            GeneralInfoBoard* generalInfoBoard = (GeneralInfoBoard*)getChildByTag(CHILD_GENERAL_INFO_TAG);
            generalInfoBoard->setGeneral(player->getGeneral());
            generalInfoBoard->addBloodBar(player->getMaxHp());
        }


        void PlayerControlBoard::setRole(Player::Role role) {
            GeneralInfoBoard* generalInfoBoard = (GeneralInfoBoard*)getChildByTag(CHILD_GENERAL_INFO_TAG);
            generalInfoBoard->setRole(role);

            PlayerTable* playerTable = (PlayerTable *)this->getParent();
            playerTable->getTableAnimationLayer()->playRoleAnimation(role);
        }

        void PlayerControlBoard::setPhase(Player::Phase phase) {
            PhaseAnimationArea* phaseAnimationArea = (PhaseAnimationArea*)getChildByTag(CHILD_PHASE_ANIMATION_AREA_TAG);
            if(phaseAnimationArea) {
                phaseAnimationArea->showAnimation(phase);
            }
        }

        void PlayerControlBoard::okTouchDownAction(CCObject* pSender, CCControlEvent event) {
            /*
            CCArray* cardArray = CARD_SELECTOR.getSelectedCards();
            int count = cardArray->count();
            sgs::layer::TableAnimationLayer* animationLayer= ((PlayerTable*)getParent())->getTableAnimationLayer();
            CardAreaBoard*  cardAreaBoard  = (CardAreaBoard* )this->getChildByTag(0);


            for(int i = 0; i <count ; i++) {
            CardItem* cardItem = (CardItem*)cardArray->objectAtIndex(i);
            animationLayer->playThrowAnimation(cardItem);

            }
            cardAreaBoard->removeCard(cardArray);
            CARD_SELECTOR.clear();
            */
        }

        void PlayerControlBoard::cancelTouchDownAction(CCObject* pSender, CCControlEvent event) {
            //PhaseAnimationArea* phaseAnimationArea = (PhaseAnimationArea*)getChildByTag(CHILD_PHASE_ANIMATION_AREA_TAG);
            //phaseAnimationArea->showDiscardAnimation();
            //CardAreaBoard*  cardAreaBoard  = (CardAreaBoard* )this->getChildByTag(0);
            //cardAreaBoard->touch();
        }
    }
}