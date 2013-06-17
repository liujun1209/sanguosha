
#include "Constants.h"
#include "LobbyScene.h"
#include "MQScene.h"
#include "WaitingRoomScene.h"
#include "UIThreadMQ.h"
#include "TableScene.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace sgs::server;
using namespace sgs::client;

namespace sgs {
    namespace scene {

        UserBoardInfo::UserBoardInfo() {
        }

        UserBoardInfo::~UserBoardInfo() {
        }

        bool UserBoardInfo::init() {
            bool bRet = false;
            do 
            {   
                //bg
                CCSprite* mainBgSprite = CCSprite::create(IMAGE_LOBBY_BOARD_BG_PATH);
                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(ccp(0,0));
                mainBgSprite->setAnchorPoint(ccp(0,0));
                this->addChild(mainBgSprite, -1);

                //头像部分
                LobbyBoardUser* user = LobbyBoardUser::create();
                user->setPosition(ccp(30, 390));
                user->setAnchorPoint(ccp(0 ,0));
                this->addChild(user);
                //按钮部分
                //创建
                CCScale9Sprite* createSprite = CCScale9Sprite::create(IMAGE_BUTTON_START_PATH);  
                CCControlButton* controlBtn = CCControlButton::create(createSprite);  
                controlBtn->setPosition(ccp(30, 200));
                controlBtn->setAnchorPoint(ccp(0, 0));
                controlBtn->setPreferredSize(CCSizeMake(168, 70));
                this->addChild(controlBtn,0, 0);
                

                CCScale9Sprite* backSprite = CCScale9Sprite::create(IMAGE_BUTTON_BACK_PATH);  
                CCControlButton* backBtn = CCControlButton::create(backSprite);  
                backBtn->setPosition(ccp(30, 120));
                backBtn->setPreferredSize(CCSizeMake(168, 70));
                backBtn->setAnchorPoint(ccp(0, 0));
                this->addChild(backBtn, 0, 1);
                
                //返回
                bRet = true;
            } while (0);
            return bRet;
        }

        

        Seat::Seat(bool host):host_(host), seated_(false) {
        }

        Seat::~Seat() {
        }

        bool Seat::init() {
            bool bRet = false;
            do {
                //bg
                CCSprite* bgSprite = CCSprite::create(IMAGE_ROOM_SEAT_OPEN_BG_PATH);
                bgSprite->setPosition(ccp(0,0));
                bgSprite->setAnchorPoint(ccp(0,0));
                this->addChild(bgSprite);

                CCSprite* headSprite = CCSprite::create(IMAGE_ROOM_SEAT_HEAD_PATH);
                headSprite->setPosition(ccp(15, 70));
                headSprite->setAnchorPoint(ccp(0 ,0));
                this->addChild(headSprite, 0 , TAG_HEAD);

                CCSprite* coverSprite = CCSprite::create(IMAGE_ROOM_SEAT_COVER_PATH);
                coverSprite->setPosition(ccp(12, 18));
                coverSprite->setAnchorPoint(ccp(0 ,0));
                this->addChild(coverSprite, 0 , TAG_COVER);

                CCLabelTTF * name = CCLabelTTF::create("",FONT_NAME_HEI, FONT_SIZE_NAME);
                name->setPosition(ccp(35, 50));
                name->setAnchorPoint(ccp(0,0));
                name->setColor(ccc3(0,0,0));
                this->addChild(name, 0 , TAG_NAME);

                CCSprite* maskSprite;
                if(host_) {
                     maskSprite = CCSprite::create(IMAGE_ROOM_SEAT_HOST_PATH);
                }
                else {
                    maskSprite = CCSprite::create(IMAGE_ROOM_SEAT_READY_PATH);
                }
                maskSprite->setPosition(ccp(85, 22));
                maskSprite->setAnchorPoint(ccp(0 ,0));
                this->addChild(maskSprite, 0 , TAG_MASK);
                
                maskSprite->setVisible(false);
                coverSprite->setVisible(false);
                headSprite->setVisible(false);
                
                bRet = true;

            }while(0);
            return bRet;
        }

        void Seat::seat(int id, const string& name){
            if(!seated_) {
                this->setId(id);
                seated_ = true;
                CCLabelTTF * label = (CCLabelTTF*)this->getChildByTag(TAG_NAME);
                label->setString(name.c_str());
                label->setVisible(true);
                CCSprite * maskSprite = (CCSprite *)this->getChildByTag(TAG_MASK);
                maskSprite->setVisible(true);
                CCSprite * coverSprite = (CCSprite *)this->getChildByTag(TAG_COVER);
                coverSprite->setVisible(true);
                CCSprite * headSprite = (CCSprite *)this->getChildByTag(TAG_HEAD);
                headSprite->setVisible(true);
            }
        }
        
        void Seat::leave() {
            if(seated_) {
                this->setId(-1);
                seated_ = false;
                CCLabelTTF * label = (CCLabelTTF*)this->getChildByTag(TAG_NAME);
                label->setVisible(false);
                CCSprite * maskSprite = (CCSprite *)this->getChildByTag(TAG_MASK);
                maskSprite->setVisible(false);
                CCSprite * coverSprite = (CCSprite *)this->getChildByTag(TAG_COVER);
                coverSprite->setVisible(false);
                CCSprite * headSprite = (CCSprite *)this->getChildByTag(TAG_HEAD);
                headSprite->setVisible(false);
            }
        }

        Seat* Seat::create(bool host) {
            Seat* pRet = new Seat(host);
            if(pRet && pRet->init()) {
                pRet->autorelease();
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }


        WaitingRoom::WaitingRoom(bool host):host_(host)
        {

        }


        WaitingRoom::~WaitingRoom(void)
        {
            if(client_) {
                client_->setWaitingRoomPtr(NULL);
            }
        }

        bool WaitingRoom::init()
        {
            bool bRet = false;
            do 
            {
                ////背景
                CCSprite* mainBgSprite = CCSprite::create(IMAGE_LOBBY_BG_PATH);
                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(ccp(0,0));
                mainBgSprite->setAnchorPoint(ccp(0,0));
                this->addChild(mainBgSprite, -1);
                //title
                CCSprite* titleSprite = CCSprite::create(IMAGE_LOBBY_TITLE_PATH);
                CC_BREAK_IF(! titleSprite);

                titleSprite->setPosition(ccp(0, 582));
                titleSprite->setAnchorPoint(ccp(0, 0));
                this->addChild(titleSprite, -1);
                //用户面板
                UserBoardInfo* userBoardInfo = UserBoardInfo::create();

                userBoardInfo->setPosition(ccp(713, 0 ));
                userBoardInfo->setAnchorPoint(ccp(0 ,0));

                CCControlButton* backBtn = (CCControlButton*)userBoardInfo->getChildByTag(1);
                backBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(WaitingRoom::backTouchDownAction), CCControlEventTouchDown);  
                
                CCControlButton* startBtn = (CCControlButton*)userBoardInfo->getChildByTag(0);
                startBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(WaitingRoom::startTouchDownAction), CCControlEventTouchDown);  

                this->addChild(userBoardInfo);

                //盟主
                Seat*  host = Seat::create(true);
                host->setPosition(ccp(40, 320));
                host->setAnchorPoint(ccp(0,0));
                this->addChild(host,0,0);
                //其他玩家
                Seat*  player1 = Seat::create(false);
                player1->setPosition(ccp(200,320));
                player1->setAnchorPoint(ccp(0,0));
                this->addChild(player1,0,1);

                Seat*  player2 = Seat::create(false);
                player2->setPosition(ccp(360,320));
                player2->setAnchorPoint(ccp(0,0));
                this->addChild(player2,0,2);

                Seat*  player3 = Seat::create(false);
                player3->setPosition(ccp(520,320));
                player3->setAnchorPoint(ccp(0,0));
                this->addChild(player3,0,3);

                Seat*  player4 = Seat::create(false);
                player4->setPosition(ccp(40,100));
                player4->setAnchorPoint(ccp(0,0));
                this->addChild(player4,0,4);

                Seat*  player5 = Seat::create(false);
                player5->setPosition(ccp(200,100));
                player5->setAnchorPoint(ccp(0,0));
                this->addChild(player5,0,5);

                Seat*  player6 = Seat::create(false);
                player6->setPosition(ccp(360,100));
                player6->setAnchorPoint(ccp(0,0));
                this->addChild(player6,0,6);

                Seat*  player7 = Seat::create(false);
                player7->setPosition(ccp(520,100));
                player7->setAnchorPoint(ccp(0,0));
                this->addChild(player7,0,7);
                if(host_) {
                    server_ = boost::shared_ptr<SgsServer>(new SgsServer());
                    server_->init();
                    server_->start();
                    //睡500毫秒 防止服务器还未建立 客户端就去建立连接
                    boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(500));

                    client_ = boost::shared_ptr<SgsClient>(new SgsClient(true));
                    client_->init();
                    client_->setWaitingRoomPtr(this);
                    client_->connect("127.0.0.1");

                }

                
                bRet = true;
            }while(0);
            return bRet;
        }

        WaitingRoom* WaitingRoom::create(bool host) {
            WaitingRoom * pRet = new WaitingRoom(host);
            if(pRet && pRet->init()) {
                pRet->autorelease();
            }
            else {
                delete pRet;
                pRet = NULL;
            }
            return pRet;
        }

        void WaitingRoom::playerJoin(int id, const string& name) {
            for(int i= 0; i<8;i++) {
                Seat * seat = (Seat*)this->getChildByTag(i);
                if(!seat->isSeated()) {
                    seat->seat(id, name);
                    break;
                }
            }
        }

        void WaitingRoom::playerExit(int id) {
            for(int i= 0; i<8;i++) {
                Seat * seat = (Seat*)this->getChildByTag(i);
                if(seat->getId() ==  id) {
                    ADD_MESSAGE(boost::bind(&Seat::leave, seat));
                    break;
                }
            }
        }

        void WaitingRoom::startGameCallback() {
            
           if(host_) {
                PlayerTable::server_ = server_;
            }
            PlayerTable::client_ = client_; 
            
            CCScene *pScene = sgs::scene::PlayerTable::scene(client_->getPlayerCount());;
            
            //关闭背景音乐
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            //切换场景
            CCDirector::sharedDirector()->replaceScene(pScene);
        }

        void WaitingRoom::startTouchDownAction(CCObject* pSender, CCControlEvent event) {
            client_->startGame();
        }

        void WaitingRoom::backTouchDownAction(CCObject* pSender, CCControlEvent event) {
            client_->disconnect();
            if(server_) {
                server_->stop();
            }
            CCDirector::sharedDirector()->popScene();
        }

        CCScene* WaitingRoom::scene(bool host)
        {
            CCScene * scene = NULL;
            do 
            {
                // 'scene' is an autorelease object
                scene = new MQScene();//CCScene::create();
                CC_BREAK_IF(! scene);

                // 'layer' is an autorelease object
                WaitingRoom *createRoom = WaitingRoom::create(host);
                CC_BREAK_IF(! createRoom);

                // add layer as a child to scene
                scene->addChild(createRoom, 0 , 0);
            } while (0);

            // return the scene
            return scene;
        }


    }
}