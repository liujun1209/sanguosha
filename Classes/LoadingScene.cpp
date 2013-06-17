
#include "TableScene.h"
#include "LoadingScene.h"

#include <boost\thread.hpp>
#include "MQScene.h"
#include "Constants.h"
#include "UIThreadMQ.h"
#include "StringResource.h"

namespace sgs {
    namespace scene {
         

        bool Loading::init() {
            bool bRet = false;
            do 
            {
                //////////////////////////////////////////////////////////////////////////
                // super init first
                //////////////////////////////////////////////////////////////////////////
                CC_BREAK_IF(! CCLayer::init());
                //背景
                CCSprite* mainBgSprite = CCSprite::create(IMAGE_LOBBY_BG_PATH);
                CC_BREAK_IF(! mainBgSprite);

                mainBgSprite->setPosition(ccp(0,0));
                mainBgSprite->setAnchorPoint(ccp(0,0));
                this->addChild(mainBgSprite, -1);

                //loading bg
                //CCLabelBMFont* label = CCLabelBMFont::create("","",50,CCTextAlignment::kCCTextAlignmentLeft);
                //CCLabelTTF* loadLabel = CCLabelTTF::create(G2U("1、奸雄：\n你可以立即获得对你造成伤害的牌\n2、护驾：\n主公技，当你需要使用（或打出）一张【闪】时，你可以发动护驾。所有魏势力角色按行动顺序依次选择是否打出一张【闪】“提供”给你（视为由你使用或打出），直到有一名角色或没有任何角色决定如此做时为止。"), "黑体", 32
                //    , ::CCSize(960,640), CCTextAlignment::kCCTextAlignmentLeft);
                CCLabelTTF* loadLabel = CCLabelTTF::create("loading", "黑体", 32);
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                loadLabel->setPosition(ccp(winSize.width/2, 60));
                loadLabel->setColor(ccc3(255,0,0));
                this->addChild(loadLabel);
                //loadLabel->runAction(::CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.0f),CCFadeIn::create(1.0f))));

                this->loadResources();
                //boost::thread th(boost::bind(&Loading::loadResources, this));

                bRet = true;
            }while(0);
            return bRet;
        }

        void Loading::loadResources() {
            
            CCSpriteFrameCache* pFrameCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
            char plistStr[50] = {0};
            char pvrStr[50] = {0};
            for(int i = 0; i< 6 ;i++) {
                sprintf(plistStr, "image/nonalpha_%d.plist", i);
                sprintf(pvrStr, "image/nonalpha_%d.png", i); 
                pFrameCache->addSpriteFramesWithFile(plistStr, pvrStr);
            }
            
            for(int i = 0; i< 2 ;i++) {
                sprintf(plistStr, "image/alpha_%d.plist", i);
                sprintf(pvrStr, "image/alpha_%d.png", i); 
                pFrameCache->addSpriteFramesWithFile(plistStr, pvrStr);
            }

            for(int i = 0; i< 2 ;i++) {
                sprintf(plistStr, "image/image_%d.plist", i);
                sprintf(pvrStr, "image/image_%d.png", i); 
                pFrameCache->addSpriteFramesWithFile(plistStr, pvrStr);
            }
            
            ADD_MESSAGE(boost::bind(&Loading::afterLoadResources, this));
        }

        void Loading::afterLoadResources() {
            
            CCSprite* mainBgSprite = CCSprite::createWithSpriteFrameName(IMAGE_TABLE_PLAYER_BG_PATH);
            
                mainBgSprite->setPosition(CCPointZero);
                this->addChild(mainBgSprite, 1, 0);
            
            //CCDirector::sharedDirector()->replaceScene(PlayerTable::scene(8));
            CCLog("Loading::afterLoadResources");
        }

        cocos2d::CCScene* Loading::scene() {
            CCScene * scene = NULL;
            do 
            {
                // 'scene' is an autorelease object
                scene = MQScene::create();
                CC_BREAK_IF(! scene);

                // 'layer' is an autorelease object
                Loading *layer = Loading::create();
                CC_BREAK_IF(! layer);

                // add layer as a child to scene
                scene->addChild(layer);
            } while (0);

            // return the scene
            return scene;
        }

    }
}