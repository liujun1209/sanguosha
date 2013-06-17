#ifndef __MQ_SCENE_H__
#define __MQ_SCENE_H__
#include <cocos2d.h>

using namespace cocos2d;

class MQScene : public CCScene
{
public:
    MQScene(void);
    ~MQScene(void);
    CREATE_FUNC(MQScene);
    void update(float dt);
};


#endif