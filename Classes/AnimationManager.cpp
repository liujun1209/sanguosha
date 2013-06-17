#include "AnimationManager.h"

#include "Constants.h"

namespace sgs {
    namespace manager {
        AnimationManager* AnimationManager::instance_ = new AnimationManager();


        AnimationManager* AnimationManager::getInstance() {
            return instance_;
        }

        AnimationManager::AnimationManager(void):init_(false)
        {
        }


        AnimationManager::~AnimationManager(void)
        {
        }



        CCAnimation* AnimationManager::getSlashAnimation() {
            CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(ANIMATION_SLASH_KEY);
            if(!animation) {
                animation = createAnimation(ANIMATION_SLASH_COUNT, ANIMATION_SLASH_FORMAT_STR, 0.1f);
                CCAnimationCache::sharedAnimationCache()->addAnimation(animation, ANIMATION_SLASH_KEY);
            }
            return animation;
        }

        CCAnimation* AnimationManager::getJinkAnimation() {
            CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(ANIMATION_JINK_KEY);
            if(!animation) {
                animation = createAnimation(ANIMATION_JINK_COUNT, ANIMATION_JINK_FORMAT_STR, 0.1f);
                CCAnimationCache::sharedAnimationCache()->addAnimation(animation, ANIMATION_JINK_KEY);
            }
            return animation;
        }

        CCAnimation* AnimationManager::getPeachAnimation() {
            CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(ANIMATION_PEACH_KEY);
            if(!animation) {
                animation = createAnimation(ANIMATION_PEACH_COUNT, ANIMATION_PEACH_FORMAT_STR, 0.1f);
                CCAnimationCache::sharedAnimationCache()->addAnimation(animation, ANIMATION_PEACH_KEY);
            }
            return animation;
        }

        CCAnimation* AnimationManager::getDamageAnimation() {
            CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(ANIMATION_DAMAGE_KEY);
            if(!animation) {
                animation = createAnimation(ANIMATION_DAMAGE_COUNT, ANIMATION_DAMAGE_FORMAT_STR, 0.1f);
                CCAnimationCache::sharedAnimationCache()->addAnimation(animation, ANIMATION_DAMAGE_KEY);
            }
            return animation;
        }

        CCAnimation* AnimationManager::createAnimation(const int& count,const char* formatStr, float unit) {
            CCSpriteFrameCache* pFrameCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
            if(!init_){
                pFrameCache->addSpriteFramesWithFile(ANIMATION_PLIST_PATH, ANIMATION_PNG_PATH);
                init_ = true;
            }
            char str[50] = {0};
            
            CCAnimation* pAnimation = CCAnimation::create();
            for(int i = 0; i != count; ++i)
            {
                sprintf(str, formatStr, i); 
                CCSpriteFrame* pFrame = pFrameCache->spriteFrameByName(str);
                pAnimation->addSpriteFrame( pFrame );
            }
            pAnimation->setRestoreOriginalFrame(true);
            pAnimation->setDelayPerUnit(unit);
            return pAnimation;
        }


    }
}