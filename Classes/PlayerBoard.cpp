
#include "TableScene.h"
#include "PlayerBoard.h"
#include "Engine.h"

using namespace sgs::scene;

namespace sgs {
    namespace ui {

        static const char* GENERAL_HEAD_FORMAT_PATH = "general_head_%s.png";
        static const char* GENERAL_KINGDOM_FORMAT_PATH = "general_kindom_%s.png";
        static const char* GENERAL_FRAME_FORMAT_PATH = "general_name_frame_%s.png";
        static const char* GENERAL_BMFONT_PATH = "bmfont_general.fnt";
        static const char* GENERAL_ROLE_FORMAT_PATH = "role_%s_s.png";
        static const char* ROLE_LORD = "lord";
        static const char* ROLE_LOYALIST = "loyalist";
        static const char* ROLE_REBEL = "rebel";
        static const char* ROLE_RENEGADE = "renegade";
        static const char* ROLE_UNKNOW = "unknow";

        static const char* BLOOD_BG_PATH = "blood_point_bg.png";
        static const char* BLOOD_1_PATH = "blood_point_1.png";
        static const char* BLOOD_3_PATH = "blood_point_2.png";
        static const char* BLOOD_5_PATH = "blood_point_3.png";

        static const char* PLAYER_BOARD_BG_PATH = "table_player.png";

        GeneralHeadItem::GeneralHeadItem(int width) : width_(width) {
        }

        bool GeneralHeadItem::init() {
            bool bRet = false;
            do {
                bRet = true;
            }while(0);

            return bRet;
        }

        GeneralHeadItem* GeneralHeadItem::create(int width) {
            GeneralHeadItem * headItem = new GeneralHeadItem(width);
            if(headItem && headItem->init()) {
                headItem->autorelease();
            }
            else {
                delete headItem;
                headItem = NULL;
            }
            return headItem;
        }

        //设置武将
        void GeneralHeadItem::setGeneral(boost::shared_ptr<General> general) {
            //頭像
            char namePath[50] = {0};
            sprintf(namePath, GENERAL_HEAD_FORMAT_PATH, general->getName().c_str());
            int originX = IMAGE_WIDTH - width_;
            originX = originX!=0 ? originX / 2: originX;
            CCSprite* head = CCSprite::create(namePath, CCRect(originX,0, width_, IMAGE_HEIGHT));
            head->setPosition(CCPointZero);
            head->setAnchorPoint(CCPointZero);
            this->addChild(head);


            //FRAME
            char framePath[50] = {0};
            sprintf(framePath, GENERAL_FRAME_FORMAT_PATH, general->getKingdom().c_str());


            CCSprite* frame = CCSprite::create(framePath);
            frame->setPosition(ccp(width_, IMAGE_HEIGHT));
            frame->setAnchorPoint(ccp(1,1));

            this->addChild(frame);

            //国标
            char kingdomPath[50] = {0};
            sprintf(kingdomPath, GENERAL_KINGDOM_FORMAT_PATH, general->getKingdom().c_str());


            CCSprite* kingdom = CCSprite::create(kingdomPath);
            kingdom->setPosition(ccp(width_ + 3, IMAGE_HEIGHT));
            kingdom->setAnchorPoint(ccp(1,1));

            this->addChild(kingdom);


            //名字
            CCLabelBMFont* chineseNameLabel = CCLabelBMFont::create(general->getChineseName().c_str(), GENERAL_BMFONT_PATH);

            chineseNameLabel->setPosition(ccp(width_ - 5, IMAGE_HEIGHT - 20));
            chineseNameLabel->setAnchorPoint(ccp(1,1));

            this->addChild(chineseNameLabel);
        }

        //设置角色
        void GeneralHeadItem::setRole(Player::Role role) {
            char rolePath[50] = {0}; 
            switch(role) {
            case Player::Lord:
                sprintf(rolePath, GENERAL_ROLE_FORMAT_PATH, ROLE_LORD);
                break;
            case Player::Loyalist:
                sprintf(rolePath, GENERAL_ROLE_FORMAT_PATH, ROLE_LOYALIST);
                break;
            case Player::Rebel:
                sprintf(rolePath, GENERAL_ROLE_FORMAT_PATH, ROLE_REBEL);
                break;
            case Player::Renegade:
                sprintf(rolePath, GENERAL_ROLE_FORMAT_PATH, ROLE_RENEGADE);
                break;
            case Player::Unknow:
                sprintf(rolePath, GENERAL_ROLE_FORMAT_PATH, ROLE_UNKNOW);
                break;
            }

            //清理
            CCSprite* temp = (CCSprite*)getChildByTag(ROLE_TAG);
            if(temp) {
                temp->removeFromParentAndCleanup(true);
            }
            //Role
            CCSprite* roleSprite = CCSprite::create(rolePath);
            roleSprite->setPosition(ccp(2, IMAGE_HEIGHT-2));
            roleSprite->setAnchorPoint(ccp(0,1));
            this->addChild(roleSprite, 3, ROLE_TAG);

        }



        Blood::Blood():level_(5) {
        }

        Blood::~Blood() {
        }

        bool Blood::init() {
            bool bRet = false;
            do {
                //血背景
                CCSprite * bloodBg = CCSprite::create(BLOOD_BG_PATH);
                CC_BREAK_IF(! bloodBg);
                bloodBg->setAnchorPoint(CCPointZero);
                bloodBg->setPosition(CCPointZero);
                this->addChild(bloodBg);

                //前景
                CCSprite * blood = CCSprite::create(BLOOD_5_PATH);
                CC_BREAK_IF(! blood);
                blood->setAnchorPoint(CCPointZero);
                blood->setPosition(ccp(3, 3));
                this->addChild(blood , 0 , 0);

                bRet = true;
            }while(0);
            return bRet;
        }

        void Blood::damage() {
            CCSprite * blood = (CCSprite *)this->getChildByTag(0);
            blood->setVisible(false);
        }

        void Blood::recover() {
            CCSprite * blood = (CCSprite *)this->getChildByTag(0);
            blood->setVisible(true);
        }

        void Blood::setLevel(int level) {
            if(level_!= level ) {
                level_ = level;
                const char * imagePath;
                switch(level_) {
                case 0:
                case 1:
                    imagePath = BLOOD_1_PATH;
                    break;
                case 2:
                    imagePath = BLOOD_3_PATH;
                    break;
                default:
                    imagePath = BLOOD_5_PATH;
                }

                CCSprite * blood = (CCSprite *)this->getChildByTag(0);
                blood->removeFromParentAndCleanup(true);
                blood = CCSprite::create(imagePath);
                blood->setAnchorPoint(CCPointZero);
                blood->setPosition(ccp(3, 3));
                this->addChild(blood , 0 , 0);
                //blood->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imagePath));
            }
        }

        BloodBar::BloodBar(int currentBlood, int maxBlood, BloodDirection direction):
        currentBlood_(currentBlood), maxBlood_(maxBlood) , direction_(direction) {
        }

        BloodBar::~BloodBar() {
        }

        bool BloodBar::init() {
            bool bRet = false;
            do {
                if( maxBlood_ >5) {

                }
                else {
                    float x, y;
                    if(direction_ == Horizontal) {
                        //水平
                        x = 33;y = 0;
                    }
                    else  {
                        //垂直
                        x = 0; y = -34;
                    }
                    for(int i = 0; i< maxBlood_; i++ ) {
                        Blood * blood = Blood::create();
                        blood->setAnchorPoint(CCPointZero);
                        blood->setPosition(ccp(i * x, i * y));
                        this->addChild(blood, 0, i);
                    }
                }
                bRet = true;
            }while(0);
            return bRet;
        }

        BloodBar* BloodBar::create(int currentBlood, int maxBlood, BloodDirection direction) {
            BloodBar * pRet = new BloodBar(currentBlood, maxBlood, direction);
            if(pRet && pRet->init()) {
                pRet->autorelease();
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }

        void BloodBar::damage(int value) {
            int newBlood = currentBlood_ - value;
            newBlood = newBlood < 0 ? 0: newBlood;
            for(int i = currentBlood_; i > newBlood; i-- ) {
                Blood* blood = (Blood *)this->getChildByTag(i - 1);
                blood->damage();
            }
            currentBlood_ = newBlood;
            setLevel();
        }

        void BloodBar::recover(int value) {
            int newBlood = currentBlood_ + value;
            newBlood = newBlood > maxBlood_ ? maxBlood_ : newBlood;
            for(int i = currentBlood_; i < newBlood; i++ ) {
                Blood* blood = (Blood *)this->getChildByTag(i);
                blood->recover();
            }
            currentBlood_ = newBlood;
            setLevel();
        }

        void BloodBar::setLevel() {
            if(currentBlood_ >0 ) {
                int level = (int)((float)currentBlood_/ maxBlood_ * 3) + 1;
                for(int i =0; i< currentBlood_; i++) {
                    Blood* blood = (Blood *)this->getChildByTag(i);
                    blood->setLevel(level);
                }
            }
        }

        int BloodBar::getCurrentBlood() {
            return currentBlood_;
        }

        void BloodBar::setCurrentBlood(int blood) {
            currentBlood_ = blood;
        }


        int BloodBar::getMaxBlood() {
            return maxBlood_;
        }

        void BloodBar::setMaxBlood(int blood) {
            maxBlood_ = blood;
        }




        PlayerBoard::PlayerBoard() {
        }

        PlayerBoard::~PlayerBoard() {
        }

        bool PlayerBoard::init() { 
            bool bRet = false;
            do {
                //背景图
                CCSprite* mainBgSprite = CCSprite::create(PLAYER_BOARD_BG_PATH);

                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(CCPointZero);
                mainBgSprite->setAnchorPoint(CCPointZero);
                this->addChild(mainBgSprite, -1, 0);

                GeneralHeadItem* generalHeadItem = GeneralHeadItem::create(90);

                generalHeadItem->setRole(Player::Unknow);
                generalHeadItem->setPosition(ccp(7, 33));
                generalHeadItem->setAnchorPoint(ccp(0,0));
                this->addChild(generalHeadItem,0, CHILD_GENERAL_HEAD_ITEM_TAG);


                //时间条
                PlayerTimeBar * timeBar =  PlayerTimeBar::create();
                timeBar->setPosition(ccp(0, -15));
                timeBar->setScaleX(0.9f);
                timeBar->setScaleY(0.7f);
                this->addChild(timeBar, 0 ,CHILD_TIME_BAR_TAG);

                //阶段
                PhaseAnimationArea* phaseAnimationArea = PhaseAnimationArea::create();
                phaseAnimationArea->setPosition(ccp(0, -68));
                phaseAnimationArea->setAnchorPoint(CCPointZero);
                this->addChild(phaseAnimationArea, 3, CHILD_PHASE_ANIMATION_AREA_TAG);


                bRet = true;
            }while(0);
            return bRet;
        }

        void PlayerBoard::touch() {
            PhaseAnimationArea* phaseAnimationArea = (PhaseAnimationArea*)getChildByTag(CHILD_PHASE_ANIMATION_AREA_TAG);
            if(phaseAnimationArea) {
                phaseAnimationArea->showPlayAnimation();
            }
        }

        void PlayerBoard::longTouch() {
            this->playRecoverAnimation(1);
        }


        void PlayerBoard::showTimeBar(float time) {
            PlayerTimeBar* timeBar = (PlayerTimeBar*)getChildByTag(CHILD_TIME_BAR_TAG);PlayerTimeBar::create();
            if(timeBar) {
                timeBar->showAnimation(time);
            }
        }

        void PlayerBoard::hiddenTimeBar() {
            PlayerTimeBar* timeBar = (PlayerTimeBar*)getChildByTag(CHILD_TIME_BAR_TAG);PlayerTimeBar::create();
            if(timeBar) {
                timeBar->hidden();
            }

        }

        
        void PlayerBoard::discardCards(set<boost::shared_ptr<Card> > cards) {
            PlayerTable* playerTable = ((PlayerTable*)getParent());
            if(playerTable) {
                CCArray itemArray;
                set<boost::shared_ptr<Card> >::iterator it;
                for(it = cards.begin(); it != cards.end(); it++) {
                    CardItem* item = CardItem::create(*it);
                    item->setPosition(CCPointZero);
                    addChild(item);
                    itemArray.addObject(item);
                }
                
                sgs::layer::TableAnimationLayer* animationLayer = playerTable->getTableAnimationLayer();
                animationLayer->playThrowAnimation(&itemArray);
            }
        }

        void PlayerBoard::drawCard(vector<boost::shared_ptr<Card> > cards) {
            PlayerTable* playerTable = ((PlayerTable*)getParent());
            if(playerTable) {
                int size = cards.size();
                playerTable->reduce(size);
                sgs::layer::TableAnimationLayer* animationLayer = playerTable->getTableAnimationLayer();
                animationLayer->playDrawCardAnimation(this->getPosition(), size);
            }
        }

        void PlayerBoard::setPlayer(boost::shared_ptr<ClientPlayer> player) {
            PlayerUI::setPlayer(player);

            GeneralHeadItem* generalHeadItem = (GeneralHeadItem*)getChildByTag(CHILD_GENERAL_HEAD_ITEM_TAG);
            generalHeadItem->setGeneral(player->getGeneral());

            BloodBar*  bloodBar = (BloodBar*)getChildByTag(BLOOD_BAR_TAG);
            if(!bloodBar) {
                int maxHp = player->getMaxHp();
                bloodBar = BloodBar::create(maxHp,maxHp , Horizontal);
                bloodBar->setPosition(ccp(6,6));
                bloodBar->setScale(0.55f);
                this->addChild(bloodBar,0, BLOOD_BAR_TAG);
            }
        }

        void PlayerBoard::setRole(Player::Role role) {
            GeneralHeadItem* generalHeadItem = (GeneralHeadItem*)getChildByTag(CHILD_GENERAL_HEAD_ITEM_TAG);
            generalHeadItem->setRole(role);
        }

        void PlayerBoard::setPhase(Player::Phase phase) {
            PhaseAnimationArea* phaseAnimationArea = (PhaseAnimationArea*)getChildByTag(CHILD_PHASE_ANIMATION_AREA_TAG);
            if(phaseAnimationArea) {
                phaseAnimationArea->showAnimation(phase);
            }
        }

        void PlayerBoard::playDamageAnimation(int value) {
            BloodBar*  bloodBar = (BloodBar*)this->getChildByTag(BLOOD_BAR_TAG);
            bloodBar->damage(value);
            playBloodAnimation("-1",ccc3(255,93,66));
        }

        void PlayerBoard::playRecoverAnimation(int value) {
            BloodBar*  bloodBar = (BloodBar*)this->getChildByTag(BLOOD_BAR_TAG);
            bloodBar->recover(value);
            playBloodAnimation("+1",ccc3(159, 215,66));
        }

        void PlayerBoard::playBloodAnimation(const char * text, ccColor3B color) {
            CCLabelBMFont* animationFont = CCLabelBMFont::create(text , "bmfont_hp.fnt");
            animationFont->setColor(color);

            this->addChild(animationFont, 1, ANIMATION_FONT_TAG);
            animationFont->runAction(CCSequence::create(CCMoveBy::create(1,ccp(0, 100)),::CCCallFunc::create(this, callfunc_selector(PlayerBoard::bloodAnimationCallback)), NULL));
        }

        void PlayerBoard::bloodAnimationCallback() {
            this->removeChildByTag(ANIMATION_FONT_TAG, true);
        }

    }
}