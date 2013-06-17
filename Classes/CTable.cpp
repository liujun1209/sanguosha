#include 
#include "Constants.h"
#include "AboutUsScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MainLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
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
    server_ =  boost::shared_ptr<sgs::net::AsioServer>(new sgs::net::AsioServer());
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
        //单机游戏
        CCMenuItemImage *pSingleGameItem = CCMenuItemImage::create(
            "image/button/singleNormal.png",
            "image/button/singleSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pSingleGameItem);
        pSingleGameItem->setTag(MENU_SINGLE_GAME_TAG);
        pSingleGameItem->setAnchorPoint(ccp(0.5,0.5));
        //多人游戏
        CCMenuItemImage *pMultiGameItem = CCMenuItemImage::create(
            "image/button/multiNormal.png",
            "image/button/multiSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pMultiGameItem);
        pMultiGameItem->setTag(MENU_MULTI_GAME_TAG);
        //关于我们
        CCMenuItemImage *pAboutUsItem = CCMenuItemImage::create(
            "image/button/aboutNormal.png",
            "image/button/aboutSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        CC_BREAK_IF(! pAboutUsItem);
        pAboutUsItem->setTag(MENU_ABOUT_GAME_TAG);
        //退出游戏
        CCMenuItemImage *pExitGameItem = CCMenuItemImage::create(
            "image/button/exitNormal.png",
            "image/button/exitSelected.png",
            this,
            menu_selector(MainLayer::menuCloseCallback));
        pExitGameItem->setTag(MENU_EXIT_GAME_TAG);
        CC_BREAK_IF(! pExitGameItem);

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pSingleGameItem, pMultiGameItem,pAboutUsItem ,pExitGameItem, NULL);
        pMenu->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
        pMenu->alignItemsVerticallyWithPadding(50);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("Version 1.0.0", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width -150, 0));
        pLabel->setAnchorPoint(ccp(0,0));
        //pLabel->setColor();

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("image/system/mainbg.jpg");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/system/springOfTheEastLogin.mp3",true);

        bRet = true;
    } while (0);

    return bRet;
}

void MainLayer::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCNode * node = (CCNode *)pSender;
    //server_->setPlayerName("test");
    //boost::thread thread(boost::bind(&sgs::net::AsioServer::bind, server_ , 8000));
    //boost::thread thread2(boost::bind(&sgs::net::AsioServer::startBoardcastLocalIp, server_ , 7000));
    string ip = "127.0.0.1";
    boost::thread thread(boost::bind(&sgs::net::AsioClient::connect, asioClient,ip ,1288));
    switch(node->getTag()) {
    case MENU_SINGLE_GAME_TAG:
        SimpleAudioEngine::sharedEngine()->playEffect("audio/system/choose-item.mp3");
        
        CCLOG("single clicked");
        break;
    case MENU_MULTI_GAME_TAG:
        SimpleAudioEngine::sharedEngine()->playEffect("audio/system/choose-item.mp3");
        CCLOG("multi clicked");
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

