#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include <cocos2d.h>
#include <string>

using namespace std;
using namespace cocos2d;

namespace sgs {
    namespace manager {

        class AnimationManager
        {
        public:
            ~AnimationManager(void);
            static AnimationManager* getInstance();
            /**
            * @获取动画
            **/
            //CCAnimation* getAnimation(const string& key);
            /**
            * @获取杀动画
            **/
            CCAnimation* getSlashAnimation();
            /**
            * @获取闪动画
            **/
            CCAnimation* getJinkAnimation();
            /**
            * @获取桃动画
            **/
            CCAnimation* getPeachAnimation();
            /**
            * @获取伤害动画
            **/
            CCAnimation* getDamageAnimation();

        private:
            static AnimationManager* instance_;
            AnimationManager(void);
            /**
            * @创建动画
            **/
            CCAnimation* createAnimation(const int& count,const char* formatStr, float unit);
        private:
            bool init_;
        };

    }
}

#endif