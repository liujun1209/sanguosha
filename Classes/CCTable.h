#ifndef __CC_TABLE__H__
#define __CC_TABLE__H__

#include "cocos2d.h"

class CCTable : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(CCTable);
private:
};

#endif  // __HELLOWORLD_SCENE_H__