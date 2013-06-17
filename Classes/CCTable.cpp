#include "CCTable.h"

using namespace cocos2d;

CCScene* CCTable::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CCTable *layer = CCTable::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CCTable::init()
{
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

        

        
        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
      
        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* bgImg = CCSprite::create("image/table/bg.png");
        CC_BREAK_IF(! bgImg);

        // Place the sprite on the center of the screen
        bgImg->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(bgImg, 0);

        CCSprite* equ = CCSprite::create("image/table/equ.png");
        CC_BREAK_IF(! equ);

        // Place the sprite on the center of the screen
        equ->setPosition(ccp(0, 0));
        equ->setAnchorPoint(ccp(0,0));
        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(equ, 0);

        CCSprite* user = CCSprite::create("image/table/user.png");
        CC_BREAK_IF(! user);

        // Place the sprite on the center of the screen
        user->setPosition(ccp(size.width-user->getTextureRect().size.width, 0));
        user->setAnchorPoint(ccp(0,0));
        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(user, 0);

         CCSprite* roles = CCSprite::create("image/table/roles.png");
        CC_BREAK_IF(! roles);

        // Place the sprite on the center of the screen
        roles->setPosition(ccp(0, size.height-roles->getTextureRect().size.height));
        roles->setAnchorPoint(ccp(0,0));
        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(roles, 0);

        bRet = true;
    } while (0);

    return bRet;
}

void CCTable::menuCloseCallback(CCObject* pSender)
{
    
}

