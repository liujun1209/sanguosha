#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include <cocos2d.h>

using namespace cocos2d;

namespace sgs {
namespace scene {

    class Loading: public cocos2d::CCLayer
    {
    public:
        // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();  

        // there's no 'id' in cpp, so we recommand to return the exactly class pointer
        static cocos2d::CCScene* scene();
    
        void afterLoadResources();
        void loadResources();
        // a selector callback
        //void menuCloseCallback(CCObject* pSender);

        // implement the "static node()" method manually
        CREATE_FUNC(Loading);
    };

}
}

#endif