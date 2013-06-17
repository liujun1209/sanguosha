#include "TableScene.h"

#include <boost\bind.hpp>

#include <cocos-ext.h>
#include "Constants.h"
#include "MQScene.h"
#include <vector>
#include <map>
#include <boost\foreach.hpp>
#include "Engine.h"
#include "MyLogger.h"
#include "StringResource.h"
#include "CharacterUtil.h"
#include "PlayerBoard.h"
#include "PlayerControllerBoard.h"


using namespace std;
using namespace cocos2d::extension;
using namespace sgs::layer;
using namespace sgs::ui;

namespace sgs {
    namespace scene {

        static const char * NUMBER_FORMAT_1 = "%d";
        static const char * NUMBER_FORMAT_2 = "0%d";
        static const char * NUMBER_FORMAT_3 = "00%d";
        static const char* ROLE_NUMBER_FORMAT = "%d/%d";
        static const char* LEFT_PILES_LABL_PATH = "label_number.png";

        boost::shared_ptr<sgs::client::SgsClient> PlayerTable::client_ = boost::shared_ptr<sgs::client::SgsClient>();
        boost::shared_ptr<sgs::server::SgsServer> PlayerTable::server_ = boost::shared_ptr<sgs::server::SgsServer>();

        CCPoint POSITIONS[7] = {ccp(730, 221), ccp(730, 366), ccp(620, 509), ccp(364, 509), ccp(117, 509) , ccp(8, 366), ccp(8, 221)};

        CCPoint PLAYER_POSITION_2[1] = {POSITIONS[3]}; 
        CCPoint PLAYER_POSITION_3[2] = {POSITIONS[1], POSITIONS[5]}; 
        CCPoint PLAYER_POSITION_4[3] = {POSITIONS[1], POSITIONS[3], POSITIONS[5]}; 
        CCPoint PLAYER_POSITION_5[4] = {POSITIONS[1], POSITIONS[2], POSITIONS[4], POSITIONS[5]}; 
        CCPoint PLAYER_POSITION_6[5] = {POSITIONS[1], POSITIONS[2], POSITIONS[3], POSITIONS[4], POSITIONS[5]}; 
        CCPoint PLAYER_POSITION_7[6] = {POSITIONS[0], POSITIONS[1], POSITIONS[2], POSITIONS[4], POSITIONS[5], POSITIONS[6]}; 
        CCPoint PLAYER_POSITION_8[7] = {POSITIONS[0], POSITIONS[1], POSITIONS[2], POSITIONS[3], POSITIONS[4], POSITIONS[5], POSITIONS[6]}; 

         map<int, CCPoint * >::value_type init_value[] =
        {
            pair<int, CCPoint *  >(2, PLAYER_POSITION_2),
            pair<int, CCPoint *  >(3, PLAYER_POSITION_3),
            pair<int, CCPoint *  >(4, PLAYER_POSITION_4),
            pair<int, CCPoint *  >(5, PLAYER_POSITION_5),
            pair<int, CCPoint *  >(6, PLAYER_POSITION_6),
            pair<int, CCPoint *  >(7, PLAYER_POSITION_7),
            pair<int, CCPoint *  >(8, PLAYER_POSITION_8)
        };

        static const map<int, CCPoint *> playerBoardPositionMap(init_value, init_value + 7);

        RolesBoard::RolesBoard() {
        }

        RolesBoard::~RolesBoard() {
        }

        bool RolesBoard::init() {
            bool bRet = false;
            do {
                //±³¾°Í¼
                CCSprite* mainBgSprite = CCSprite::create(IMAGE_TABLE_ROLES_PATH);
                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(CCPointZero);
                mainBgSprite->setAnchorPoint(CCPointZero);
                this->addChild(mainBgSprite, -1);

                CCLabelTTF * lord = CCLabelTTF::create("", "ºÚÌå", 26);
                lord->setAnchorPoint(CCPointZero);
                lord->setPosition(ccp(36, 107));
                lord->setColor(ccc3(247,227,148));
                this->addChild(lord, 0, CHILD_LABEL_LORD_TAG);

                CCLabelTTF * zhong = CCLabelTTF::create("", "ºÚÌå", 26);
                zhong->setAnchorPoint(CCPointZero);
                zhong->setPosition(ccp(36, 73));
                zhong->setColor(ccc3(247,227,148));
                this->addChild(zhong, 0 ,CHILD_LABEL_LOYALIST_TAG);

                CCLabelTTF * fan = CCLabelTTF::create("", "ºÚÌå", 26);
                fan->setAnchorPoint(CCPointZero);
                fan->setPosition(ccp(36, 37));
                fan->setColor(ccc3(247,227,148));
                this->addChild(fan, 0, CHILD_LABEL_REBEL_TAG);

                CCLabelTTF * nei = CCLabelTTF::create("", "ºÚÌå", 26);
                nei->setAnchorPoint(CCPointZero);
                nei->setPosition(ccp(36, 5));
                nei->setColor(ccc3(247,227,148));
                this->addChild(nei, 0, CHILD_LABEL_RENEGADE_TAG);

                bRet = true;
            }while(0);
            return bRet;
        }

        void RolesBoard::reset(int lord, int loyalist, int rebel, int renegade) {
            lordNum_ = lord;
            lordMaxNum_ = lord;
            loyalistNum_ = loyalist;
            loyalistMaxNum_ = loyalist;
            rebelNum_ = rebel;
            rebelMaxNum_ = rebel;
            renegadeNum_ = renegade;
            renegadeMaxNum_ = renegade;
            update(Player::Lord);
            update(Player::Rebel);
            update(Player::Loyalist);
            update(Player::Renegade);
        }

        void RolesBoard::dead(Player::Role role) {
            switch(role) {
            case Player::Lord:
                lordNum_--;
                return;
            case Player::Rebel:
                rebelNum_--;
                return;
            case Player::Loyalist:
                loyalistNum_--;
                return;
            case Player::Renegade:
                renegadeNum_--;
                return;
            }
            update(role);
        }

        void RolesBoard::update(Player::Role role) {
            CCLabelTTF * label;
            char text[10] = {0};
            switch(role) {
            case Player::Lord:
                label = (CCLabelTTF *)getChildByTag(CHILD_LABEL_LORD_TAG);
                sprintf(text, ROLE_NUMBER_FORMAT, lordNum_,lordMaxNum_);
                break;
            case Player::Rebel:
                label = (CCLabelTTF *)getChildByTag(CHILD_LABEL_REBEL_TAG);
                sprintf(text, ROLE_NUMBER_FORMAT, rebelNum_, rebelMaxNum_);
                break;
            case Player::Loyalist:
                label = (CCLabelTTF *)getChildByTag(CHILD_LABEL_LOYALIST_TAG);
                sprintf(text, ROLE_NUMBER_FORMAT, loyalistNum_, loyalistMaxNum_);
                break;
            case Player::Renegade:
                label = (CCLabelTTF *)getChildByTag(CHILD_LABEL_RENEGADE_TAG);
                sprintf(text, ROLE_NUMBER_FORMAT, renegadeNum_, renegadeMaxNum_);
                break;
            }
            if(label) {
                label->setString(text);
            }
        }

        LeftPilesBoard::LeftPilesBoard():leftPiles_(0) {
        }

        LeftPilesBoard::~LeftPilesBoard() {
        }

        bool LeftPilesBoard::init() {
            bool bRet = false;
            do {
                //±³¾°Í¼
                CCSprite* mainBgSprite = CCSprite::create(IMAGE_TABLE_PILES_PATH);
                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(CCPointZero);
                mainBgSprite->setAnchorPoint(CCPointZero);
                this->addChild(mainBgSprite, -1);

                CCLabelAtlas * label = CCLabelAtlas::create("000",LEFT_PILES_LABL_PATH , 13 ,20, '0');
                label->setAnchorPoint(CCPointZero);
                label->setPosition(ccp(52, 8));
                addChild(label, 0, CHILD_NUMBER_LABEL_TAG);

                bRet = true;
            }while(0);
            return bRet;
        }

        void LeftPilesBoard::reduce(int num) {
            setLeftPiles(leftPiles_ - num);
        }

        void LeftPilesBoard::setLeftPiles(int num) {
            leftPiles_ = num;
            leftPiles_ = leftPiles_ < 0 ? 0: leftPiles_;
            CCLabelAtlas* label = (CCLabelAtlas*)getChildByTag(CHILD_NUMBER_LABEL_TAG);

            if(label) {
                char num[10] = {0};
                if(leftPiles_>=100) {
                    sprintf(num,NUMBER_FORMAT_1,leftPiles_);
                }else if(leftPiles_ < 100 && leftPiles_ >= 10) {
                    sprintf(num,NUMBER_FORMAT_2,leftPiles_);
                }else if(leftPiles_<10) {
                    sprintf(num,NUMBER_FORMAT_3,leftPiles_);
                }
                label->setString(num);
            }
        }

        PlayerTable::PlayerTable(int count) : playerCount_(count), ready_(false){
        }

        PlayerTable::~PlayerTable() {
        }

        void PlayerTable::onEnter () {
            CCLayer::onEnter();
            if(!ready_ && client_) {
                client_->ready();
            }
        }

        bool PlayerTable::init() {
            bool bRet = false;
            do {
                CC_BREAK_IF(! CCLayer::init());

                //±³¾°Í¼
                CCSprite* mainBgSprite = CCSprite::create(IMAGE_TABLE_BG_PATH);
                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(CCPointZero);
                mainBgSprite->setAnchorPoint(CCPointZero);
                this->addChild(mainBgSprite, -1);


                //²Ëµ¥ 856 y:587
                CCScale9Sprite* menuSprite = CCScale9Sprite::create(IMAGE_TABLE_MENU_PATH);  
                CCControlButton* menuBtn = CCControlButton::create(menuSprite);  
                menuBtn->setPosition(ccp(856, 587));
                menuBtn->setPreferredSize(CCSizeMake(89, 36));
                menuBtn->setAnchorPoint(ccp(0, 0));
                this->addChild(menuBtn);

                //piles 856 y:507
                LeftPilesBoard* piles = LeftPilesBoard::create();
                piles->setAnchorPoint(CCPointZero);
                piles->setPosition(ccp( 856,507));
                this->addChild(piles, 0, CHILD_LEFT_PILES_TAG);

                //roles 8 y:496
                RolesBoard * roles = RolesBoard::create();
                roles->setAnchorPoint(CCPointZero);
                roles->setPosition(ccp(8, 496));
                this->addChild(roles, 0, CHILD_ROLE_BOARD_TAG);

                //player  x:8, 117,364, 620, 730 y:509 366 221
                for(int i = 0; i < playerCount_ - 1; i++) {
                    PlayerBoard * player = PlayerBoard::create();
                    player->setAnchorPoint(CCPointZero);
                    player->setPosition(getPlayerBoardPosition(i));
                    player->setTag(i + 1);
                    this->addChild(player);
                }



                //controller
                PlayerControlBoard * controlBoard = PlayerControlBoard::create();
                controlBoard->setAnchorPoint(CCPointZero);
                controlBoard->setPosition(ccp(0, 0));
                this->addChild(controlBoard,0,0);

                if(client_) {
                    PlayerTable::client_->setPlayerTablePtr(this);
                }

                bRet = true;

            } while(0);
            return bRet;
        }


        void PlayerTable::reduce(int num) {
            LeftPilesBoard* piles = (LeftPilesBoard*)getChildByTag(CHILD_LEFT_PILES_TAG);
            if(piles) {
                piles->reduce(num);
            }
        }

        void PlayerTable::setLeftPiles(int num) {
            LeftPilesBoard* piles = (LeftPilesBoard*)getChildByTag(CHILD_LEFT_PILES_TAG);
            if(piles) {
                piles->setLeftPiles(num);
            }
        }
        
        void PlayerTable::setRoleInfo(int lord, int loyalist, int rebel, int renegade){
            RolesBoard * roleBoard = (RolesBoard *)getChildByTag(CHILD_ROLE_BOARD_TAG);
            if(roleBoard) {
                roleBoard->reset(lord, loyalist, rebel, renegade);
            }
        }

        PlayerUI* PlayerTable::getPlayerSeat(int seat) {
            PlayerUI* playerUI = dynamic_cast<PlayerUI*>(getChildByTag(seat));
            return playerUI;
        }

        CCPoint PlayerTable::getPlayerBoardPosition(int index) {
            map<int, CCPoint *>::const_iterator it = playerBoardPositionMap.find(playerCount_);
            if(it != playerBoardPositionMap.end()) {
                return (it->second[index]);
            }
            return CCPointZero;//getPlayerBoardPositionMap(index, playerCount_);
        }

        PlayerTable* PlayerTable::create(int count) {
            PlayerTable * pRet = new PlayerTable(count);
            if(pRet && pRet->init()) {
                pRet->autorelease();
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }

        TableAnimationLayer* PlayerTable::getTableAnimationLayer() {
            return (TableAnimationLayer*)(getParent()->getChildByTag(ANIMATION_LAYER_TAG));
        }

        cocos2d::CCScene* PlayerTable::scene(int count) {
            CCScene * scene = NULL;
            do 
            {
                // 'scene' is an autorelease object
                scene = new MQScene();//CCScene::create();
                CC_BREAK_IF(! scene);

                // 'layer' is an autorelease object
                PlayerTable *createRoom = PlayerTable::create(count);
                CC_BREAK_IF(! createRoom);

                // add layer as a child to scene
                scene->addChild(createRoom, 0 , 0);

                TableAnimationLayer* animationlayer = TableAnimationLayer::create();
                animationlayer->setAnchorPoint(CCPointZero);
                animationlayer->setPosition(CCPointZero);

                scene->addChild(animationlayer, 0, ANIMATION_LAYER_TAG);

            } while (0);

            // return the scene
            return scene;
        }

    }
}