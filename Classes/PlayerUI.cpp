#include "PlayerUI.h"

namespace sgs {
    namespace ui {

        static const char* IMAGE_TABLE_PLAYER_PHASE_JUDGE_PATH = "phase_panding.png";
        static const char* IMAGE_TABLE_PLAYER_PHASE_DRAW_PATH = "phase_mopai.png";
        static const char* IMAGE_TABLE_PLAYER_PHASE_PLAY_PATH = "phase_chupai.png";
        static const char* IMAGE_TABLE_PLAYER_PHASE_DISCARD_PATH = "phase_qipai.png";
        static const char* IMAGE_TABLE_PLAYER_PHASE_FINISH_PATH = "phase_end.png";


        static const char* IMAGE_TABLE_PLAYER_TIMEBAR_PATH = "system_MyTimerBar.png";
        static const char* IMAGE_TABLE_PLAYER_TIMEBAR_BG_PATH = "system_MyTimerBarBG.png";

        PlayerUI::PlayerUI(void)
        {
        }


        PlayerUI::~PlayerUI(void)
        {
        }


        void PlayerUI::setPlayer(boost::shared_ptr<ClientPlayer> player) {
            player_ = player;
            player_->setPlayerUI(this);
        }
        /*
        void PlayerUI::setRole(Player::Role role) {

        }
        */

        PhaseAnimationArea::PhaseAnimationArea() {
        }

        PhaseAnimationArea::~PhaseAnimationArea() {
        }

        bool PhaseAnimationArea::init() {
            bool bRet = false;
            do 
            {
                judgeSpritePtr_ =  CCSprite::create(IMAGE_TABLE_PLAYER_PHASE_JUDGE_PATH);
                CC_BREAK_IF(! judgeSpritePtr_);
                judgeSpritePtr_->setAnchorPoint(CCPointZero);
                judgeSpritePtr_->setPosition(CCPointZero);
                judgeSpritePtr_->setOpacity(0);
                this->addChild(judgeSpritePtr_);

                drawSpritePtr_ =  CCSprite::create(IMAGE_TABLE_PLAYER_PHASE_DRAW_PATH);
                CC_BREAK_IF(! drawSpritePtr_);
                drawSpritePtr_->setAnchorPoint(CCPointZero);
                drawSpritePtr_->setPosition(CCPointZero);
                drawSpritePtr_->setOpacity(0);
                this->addChild(drawSpritePtr_);


                playSpritePtr_ =  CCSprite::create(IMAGE_TABLE_PLAYER_PHASE_PLAY_PATH);
                CC_BREAK_IF(! playSpritePtr_);
                playSpritePtr_->setAnchorPoint(CCPointZero);
                playSpritePtr_->setPosition(CCPointZero);
                playSpritePtr_->setOpacity(0);
                this->addChild(playSpritePtr_);


                discardSpritePtr_ =  CCSprite::create(IMAGE_TABLE_PLAYER_PHASE_DISCARD_PATH);
                CC_BREAK_IF(! discardSpritePtr_);
                discardSpritePtr_->setAnchorPoint(CCPointZero);
                discardSpritePtr_->setPosition(CCPointZero);
                discardSpritePtr_->setOpacity(0);
                this->addChild(discardSpritePtr_);


                finishSpritePtr_ =  CCSprite::create(IMAGE_TABLE_PLAYER_PHASE_FINISH_PATH);
                CC_BREAK_IF(! finishSpritePtr_);
                finishSpritePtr_->setAnchorPoint(CCPointZero);
                finishSpritePtr_->setPosition(CCPointZero);
                finishSpritePtr_->setOpacity(0);
                this->addChild(finishSpritePtr_);

                bRet = true;
            }while(0);
            return bRet;
        }

        void PhaseAnimationArea::showAnimation(CCSprite* sprite) {
            if(sprite) {
                sprite->stopAllActions();
                sprite->setPosition(CCPointZero);
                sprite->setOpacity(255);
                sprite->runAction(CCSpawn::create(CCMoveTo::create(1.5,ccp(120,0)), ::CCFadeOut::create(2),NULL));
            }
        }


        /**
        * @brief ²¥·ÅÅÐ¶¨½×¶Î
        */
        void  PhaseAnimationArea::showJudgeAnimation() {
            showAnimation(judgeSpritePtr_);
        }
        /**
        * @brief ²¥·Å×¥ÅÆ½×¶Î
        */
        void  PhaseAnimationArea::showDrawAnimation() {
            showAnimation(drawSpritePtr_);
        }
        /**
        * @brief ²¥·Å·¢ÅÆ½×¶Î
        */
        void  PhaseAnimationArea::showPlayAnimation() {
            showAnimation(playSpritePtr_);
        }
        /**
        * @brief ²¥·ÅÆúÅÆ½×¶Î
        */
        void  PhaseAnimationArea::showDiscardAnimation(){
            showAnimation(discardSpritePtr_);
        }
        /**
        * @brief ²¥·Å»ØºÏ½áÊø
        */
        void  PhaseAnimationArea::showFinishAnimation() {
            showAnimation(finishSpritePtr_);
        }

        void PhaseAnimationArea::showAnimation(Player::Phase phase) {
            switch(phase) {
            case Player::Judge:
                showJudgeAnimation();
                return;
            case Player::Draw:
                showDrawAnimation();
                return;
            case Player::Play:
                showPlayAnimation();
                return;
            case Player::Discard:
                showDiscardAnimation();
                return;
            case Player::Finish:
                showFinishAnimation();
                return;
            }
        }

        PlayerTimeBar::PlayerTimeBar() {
        }

        PlayerTimeBar::~PlayerTimeBar() {
        }

        bool PlayerTimeBar::init() {
            bool bRet = false;
            do{
                //±³¾°
                CCSprite* bgSpritePtr =  CCSprite::create(IMAGE_TABLE_PLAYER_TIMEBAR_BG_PATH);
                CC_BREAK_IF(! bgSpritePtr);
                bgSpritePtr->setAnchorPoint(CCPointZero);
                bgSpritePtr->setPosition(CCPointZero);
                this->addChild(bgSpritePtr);
                //timeÌõ
                CCSprite* timeSpritePtr =  CCSprite::create(IMAGE_TABLE_PLAYER_TIMEBAR_PATH);
                CC_BREAK_IF(! timeSpritePtr);
                timeSpritePtr->setAnchorPoint(CCPointZero);
                timeSpritePtr->setPosition(CCPointZero);

                CCProgressTimer * timer = CCProgressTimer::create(timeSpritePtr);
                timer->setAnchorPoint(CCPointZero);
                timer->setPosition(ccp(5,9));

                timer->setType(kCCProgressTimerTypeBar);
                timer->setMidpoint(CCPointZero);
                timer->setBarChangeRate(ccp(1,0));
                this->addChild(timer, 0, 1);
                this->setVisible(false);

                //this->showAnimation(20);

                bRet = true;
            }while(0);
            return bRet;
        }

        void PlayerTimeBar::showAnimation(float time) {
            this->setVisible(true);
            CCProgressTimer * timer = (CCProgressTimer *)this->getChildByTag(1);
            timer->setPercentage(100);
            timer->runAction(::CCSequence::create(CCProgressFromTo::create(time,100,0),::CCCallFunc::create(this, callfunc_selector(PlayerTimeBar::hidden)), NULL));
        }

        void PlayerTimeBar::cancelAnimation() {
            CCProgressTimer * timer = (CCProgressTimer *)this->getChildByTag(1);
            timer->stopAllActions();
        }

        void PlayerTimeBar::hidden() {
            this->setVisible(false);
        }
    }
}