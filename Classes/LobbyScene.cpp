#include "LobbyScene.h"
#include "MQScene.h"
#include "Constants.h"
#include "Config.h"
#include "UIThreadMQ.h"
#include "StringResource.h"
#include "WaitingRoomScene.h"


using namespace cocos2d;
using namespace sgs::scene;

namespace sgs {
    namespace scene {

        LobbyBoardUser::LobbyBoardUser() {
        }

        LobbyBoardUser::~LobbyBoardUser() {

        }

        bool LobbyBoardUser::init() {
            bool bRet = false;
            do 
            {   
                //bg
                CCSprite* mainBgSprite = CCSprite::create(IMAGE_LOBBY_USER_BG_PATH);
                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(ccp(0,0));
                mainBgSprite->setAnchorPoint(ccp(0,0));
                this->addChild(mainBgSprite, -1);
                //头像
                CCSprite* headSprite = CCSprite::create(IMAGE_LOBBY_USER_HEAD_PATH);
                CC_BREAK_IF(! headSprite);

                headSprite->setPosition(ccp(5, 63));
                headSprite->setAnchorPoint(ccp(0,0));
                this->addChild(headSprite, 0);
                //名字
                CCLabelTTF * nameLabel = CCLabelTTF::create(sgs::utils::Config::getInstance()->getProperty(CONFIG_KEY_NAME).c_str(),FONT_NAME_SONG ,FONT_SIZE_NAME);
                nameLabel->setColor(ccc3(255,255,0));
                nameLabel->setPosition(ccp(5, 30));
                nameLabel->setAnchorPoint(ccp(0, 0));
                this->addChild(nameLabel, 1);
                bRet = true;
            } while (0);
            return bRet;
        }

        LobbyBoardInfo::LobbyBoardInfo() {
        }

        LobbyBoardInfo::~LobbyBoardInfo() {

        }

        bool LobbyBoardInfo::init() {
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
                CCScale9Sprite* createSprite = CCScale9Sprite::create(IMAGE_BUTTON_CREATE_PATH);  
                CCControlButton* controlBtn = CCControlButton::create(createSprite);  
                controlBtn->setPosition(ccp(30, 200));
                controlBtn->setAnchorPoint(ccp(0, 0));
                controlBtn->setPreferredSize(CCSizeMake(168, 70));
                this->addChild(controlBtn);
                controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LobbyBoardInfo::createTouchDownAction), CCControlEventTouchUpInside);  

                CCScale9Sprite* backSprite = CCScale9Sprite::create(IMAGE_BUTTON_BACK_PATH);  
                CCControlButton* backBtn = CCControlButton::create(backSprite);  
                backBtn->setPosition(ccp(30, 120));
                backBtn->setPreferredSize(CCSizeMake(168, 70));
                backBtn->setAnchorPoint(ccp(0, 0));
                this->addChild(backBtn);
                backBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LobbyBoardInfo::backTouchDownAction), CCControlEventTouchUpInside);  

                //返回
                bRet = true;
            } while (0);
            return bRet;
        }
        void LobbyBoardInfo::createTouchDownAction(CCObject* pSender, CCControlEvent event) {
            CCDirector::sharedDirector()->pushScene(WaitingRoom::scene(true));
        }

        void LobbyBoardInfo::backTouchDownAction(CCObject* pSender, CCControlEvent event) {
            CCDirector::sharedDirector()->popScene();
        }


        LobbyTableCell * LobbyTableCell::create(const CCSize& size) {
            LobbyTableCell* pRet = new LobbyTableCell(size);
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

        bool LobbyTableCell::isSelect() {
            return selected_;
        }

        void LobbyTableCell::setRoomName(const string& name) {
            if(isSelectable() && name.empty()) {
                lable_->setString("");
            }
            else {
                int num = getIdx() + 1;
                lable_->setString((name + STRING_RESOURCE(STRING_KEY_LOBBY_ROOM_NAME)).c_str());
                //lable_->setString(name.c_str());
            }
        }

        LobbyTableCell::LobbyTableCell(const CCSize& size) : cellSize_(size), selected_(false), selectable_(false) {
        }

        LobbyTableCell::~LobbyTableCell() {

        }

        void LobbyTableCell::select() {
            if(!selected_) {
                unselectBg_->setVisible(false);
                selectBg_->setVisible(true);
                selected_= true;
            }
        }
        void LobbyTableCell::unselect() {
            if(selected_) {
                unselectBg_->setVisible(true);
                selectBg_->setVisible(false);
                selected_= false;
            }
        }

        bool LobbyTableCell::init() {
            bool bRet = false;
            do 
            {
                //背景
                unselectBg_ = CCSprite::create(IMAGE_LOBBY_TABLE_CELL_BG_PATH);
                CC_BREAK_IF(! unselectBg_);
                //unselectBg_->setPosition(ccp(0,0));
                unselectBg_->setAnchorPoint(ccp(0 , 0.5));
                unselectBg_->setPosition(ccp(0, cellSize_.height/2));
                this->addChild(unselectBg_);
                selectBg_ = CCSprite::create(IMAGE_LOBBY_TABLE_CELL_SELECT_BG_PATH);
                CC_BREAK_IF(! selectBg_);
                selectBg_->setAnchorPoint(ccp(0 , 0.5));
                selectBg_->setPosition(ccp(0, cellSize_.height/2));
                this->addChild(selectBg_);
                selectBg_->setVisible(false);

                lable_ = CCLabelTTF::create("", "宋体", 42);

                lable_->setPosition(ccp(20, cellSize_.height/2));
                lable_->setAnchorPoint(ccp(0,0.5));
                this->addChild(lable_);

                bRet = true;
            } while (0);
            return bRet;
        }


        CCScene* Lobby::scene()
        {
            CCScene * scene = NULL;
            do 
            {
                // 'scene' is an autorelease object
                scene = new MQScene();//CCScene::create();
                CC_BREAK_IF(! scene);

                // 'layer' is an autorelease object
                Lobby *lobby = Lobby::create();
                CC_BREAK_IF(! lobby);

                // add layer as a child to scene
                scene->addChild(lobby);
            } while (0);

            // return the scene
            return scene;
        }


        Lobby::Lobby(void):cellSize_(CCSizeMake(600, 80)),maxCount_(6)
        {
        }


        Lobby::~Lobby(void)
        {
        }

        bool Lobby::init()
        {
            bool bRet = false;
            do 
            {
                CC_BREAK_IF(! CCLayer::init());
                //背景
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
                //房间table

                tableView_ = CCTableView::create(this, CCSizeMake(600,500));
                tableView_->setDirection(kCCScrollViewDirectionVertical);
                tableView_->setPosition(ccp(50,70));
                tableView_->setAnchorPoint(ccp(0, 0));
                tableView_->setDelegate(this);
                tableView_->setVerticalFillOrder(kCCTableViewFillTopDown);

                this->addChild(tableView_);
                tableView_->reloadData();

                //用户面板
                LobbyBoardInfo* LobbyBoardInfo = LobbyBoardInfo::create();

                LobbyBoardInfo->setPosition(ccp(713, 0 ));
                LobbyBoardInfo->setAnchorPoint(ccp(0 ,0));

                this->addChild(LobbyBoardInfo);


                //初始化
                seacherPtr_ = boost::shared_ptr<UdpSearcher>(new UdpSearcher());
                seacherPtr_->setCallback(boost::bind(&Lobby::seacherCallback, this, _1));
                //开始查找
                boost::thread serchThread(boost::bind(&UdpSearcher::search, seacherPtr_));

                client_ =  boost::shared_ptr<sgs::client::SgsClient>(new sgs::client::SgsClient());
                client_->setLobbyPtr(this);
                client_->init();
                bRet = true;
            } while (0);

            return bRet;
        }

        void Lobby::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell) {
            LobbyTableCell* lobbyCell = (LobbyTableCell*)cell;
            if(lobbyCell->isSelectable()) {
                client_->connect(lobbyCell->getIp());
            }
        }

        cocos2d::CCSize Lobby::cellSizeForTable(cocos2d::extension::CCTableView *table) {
            return cellSize_;
        }

        cocos2d::extension::CCTableViewCell* Lobby::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx) {

            CCTableViewCell *cell = table->dequeueCell();
            if (!cell) {
                cell = LobbyTableCell::create(cellSize_);

            }
            return cell;
        }

        unsigned int Lobby::numberOfCellsInTableView(cocos2d::extension::CCTableView *table) {
            return maxCount_;
        }

        /**
        * @brief 搜索回调函数，不能直接调用UIThread接口, 通过UIThreadMQ调用
        **/
        void Lobby::seacherCallback(const sgs::net::IpAndNameStruct& ipAndName) {
            if(ipAndName.status == sgs::net::IpAndNameStruct::Recived) {
                UIThreadMQ::getInstance()->addMessage(boost::bind(&Lobby::addRoom, this, ipAndName.name, ipAndName.ip));
            }
        }

        void Lobby::authSuccessCallback(){
            CCScene * scene = WaitingRoom::scene(false);
            WaitingRoom * waitingRoom = (WaitingRoom *)scene->getChildByTag(0);
            waitingRoom->setClient(this->client_);
            client_->setWaitingRoomPtr(waitingRoom);
            CCDirector::sharedDirector()->pushScene(scene);
            client_->requestRoomInfo();
            //client_.reset();
        }

        void Lobby::onEnter () {
            CCLayer::onEnter();
            //清空

        }
        void Lobby::onExit (){
            CCLayer::onExit();
            //停止查找
        }

        void Lobby::addRoom(const string& name, const string& ip) {
            for(int i = 0; i<maxCount_; i++ ) {
                LobbyTableCell*  cell = (LobbyTableCell*)tableView_->cellAtIndex(i);
                if(cell){
                    if(!cell->isSelectable()) {
                        cell->setSelectable(true);
                        cell->setRoomName(name);
                        cell->setIp(ip);
                        tableView_->reloadData();
                        break;
                    }
                    else if(cell->getIp() == ip) {
                        break;
                    }
                }
            }
        }

    }
}