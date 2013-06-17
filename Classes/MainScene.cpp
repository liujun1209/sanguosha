#include "MainScene.h"
#include "Constants.h"
#include "AboutUsScene.h"
#include "CCTable.h"
#include "Config.h"
#include <cocos-ext.h>
#include "SgsDialog.h"
#include "StringResource.h"
#include "MQScene.h"
#include "UIThreadMQ.h"
#include "LobbyScene.h"

using namespace sgs::client;
using namespace sgs::utils;
using namespace sgs::ui;
using namespace cocos2d;
using namespace CocosDenshion;

using namespace cocos2d::extension;

CCScene* MainLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = new MQScene();//CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MainLayer *layer = MainLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainLayer::init()
{
    //server_ =  boost::shared_ptr<sgs::net::AsioServer>(new sgs::net::AsioServer());
    //server_->setIoHandler(boost::shared_ptr<IoHandler>(new ServerIoHandler()));
    //asioClient.setIoHandler(boost::shared_ptr<IoHandler>(new ClientIoHandler()));
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.



        // Add the menu to HelloWorld layer as a child layer.
        //this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        string version = "Version " + Config::getInstance()->getProperty(CONFIG_KEY_VERSION);
        CCLabelTTF* pLabel = CCLabelTTF::create(version.c_str(), "Arial", 24);
        CC_BREAK_IF(! pLabel);
        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width -150, 0));
        pLabel->setAnchorPoint(ccp(0,0));
        //pLabel->setColor();

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1, 99);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("system_mainbg.jpg");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/system/springOfTheEastLogin.mp3",true);
        initMenu();
        //是否有名字
        bool hasName = Config::getInstance()->containProperty(CONFIG_KEY_NAME);
        if(!hasName) {
            pMenu->setVisible(false);
            showInputNameDialog();
        }
        

        bRet = true;
    } while (0);

    return bRet;
}

void MainLayer::initMenu() {

    do 
    {
        //单机游戏
        CCMenuItemImage *pSingleGameItem = CCMenuItemImage::create(
            "button_singleNormal.png",
            "button_singleSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pSingleGameItem);
        pSingleGameItem->setTag(MENU_SINGLE_GAME_TAG);
        pSingleGameItem->setAnchorPoint(ccp(0.5,0.5));
        //多人游戏
        CCMenuItemImage *pMultiGameItem = CCMenuItemImage::create(
            "button_multiNormal.png",
            "button_multiSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pMultiGameItem);
        pMultiGameItem->setTag(MENU_MULTI_GAME_TAG);
        //关于我们
        CCMenuItemImage *pAboutUsItem = CCMenuItemImage::create(
            "button_aboutNormal.png",
            "button_aboutSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pAboutUsItem);
        pAboutUsItem->setTag(MENU_ABOUT_GAME_TAG);
        //退出游戏
        CCMenuItemImage *pExitGameItem = CCMenuItemImage::create(
            "button_exitNormal.png",
            "button_exitSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        pExitGameItem->setTag(MENU_EXIT_GAME_TAG);
        CC_BREAK_IF(! pExitGameItem);

        // Create a menu with the "close" menu item, it's an auto release object.
        pMenu = CCMenu::create(pSingleGameItem, pMultiGameItem,pAboutUsItem ,pExitGameItem, NULL);
        pMenu->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
        pMenu->alignItemsVerticallyWithPadding(50);
        CC_BREAK_IF(! pMenu);
        
        this->addChild(pMenu);
    }
    while(0);
}

void MainLayer::inputName(const string& name) {
    if(!name.empty()) {
        Config::getInstance()->setProperty(CONFIG_KEY_NAME, name);
        this->removeChildByTag(MENU_INPUT_NAME_TAG,true);
        pMenu->setVisible(true);
    }
}

void MainLayer::showInputNameDialog() {
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    SgsInputDialog* dialog = SgsInputDialog::create(CCSizeMake(400, 200),StringResource::getInstance()->getString(STRING_KEY_INPUT));
    dialog->setPosition(size.width/2, size.height/2);
    dialog->setAnchorPoint(ccp(0.5 ,0.5));
    dialog->setCallbackFunc(boost::bind(&MainLayer::inputName,this, _1));
    dialog->setTag(MENU_INPUT_NAME_TAG);
    this->addChild(dialog);

}

void MainLayer::startServer() {

    //boost::thread thread(boost::bind(&sgs::net::AsioServer::bind, server_ , NET_SERVER_PORT));
    //boost::thread thread2(boost::bind(&sgs::net::AsioServer::startBoardcastLocalIp, server_ , NET_BROADCAST_PORT));
}

void MainLayer::startClient() {

    //string ip = "127.0.0.1";
    //boost::thread thread(boost::bind(&sgs::net::AsioClient::connect, &asioClient,ip ,NET_SERVER_PORT, 1000));
    //UdpSearcher * searcher = new UdpSearcher();
    //searcher->setCallback(boost::bind(&MainLayer::clientCallback, this, _1));
    //boost::thread thread(boost::bind(&sgs::net::UdpSearcher::search, searcher));
}

void MainLayer::clientCallback(const IpAndNameStruct& ipname) {
    if(ipname.status == IpAndNameStruct::Recived) {
        ::UIThreadMQ::getInstance()->addMessage(boost::bind(&MainLayer::modifyString, this, ipname.name));
    }
}
void MainLayer::modifyString(string name) {
     CCLabelTTF* pLabel = (CCLabelTTF*)this->getChildByTag(99);
     pLabel->setString(name.c_str());
}

void MainLayer::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCNode * node = (CCNode *)pSender;

    switch(node->getTag()) {
    case MENU_SINGLE_GAME_TAG:
        SimpleAudioEngine::sharedEngine()->playEffect("audio/system/choose-item.mp3");
        //startServer();
        CCLOG("single clicked");
        //CCDirector::sharedDirector()->pushScene(CCTable::scene());
        break;
    case MENU_MULTI_GAME_TAG:
        SimpleAudioEngine::sharedEngine()->playEffect("audio/system/choose-item.mp3");
        //startClient();
        CCLOG("multi clicked");
        CCDirector::sharedDirector()->pushScene(sgs::scene::Lobby::scene());

        break;
    case MENU_ABOUT_GAME_TAG:
        SimpleAudioEngine::sharedEngine()->playEffect("audio/system/choose-item.mp3");
        CCLOG("about clicked");
        CCDirector::sharedDirector()->pushScene(AboutUs::scene());
        break;
    case MENU_EXIT_GAME_TAG:
        CCDirector::sharedDirector()->end();
        break;
    }
}

