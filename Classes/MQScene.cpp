#include "MQScene.h"
#include "UIThreadMQ.h"

MQScene::MQScene(void)
{
    CCScene();
    this->scheduleUpdate();
}


MQScene::~MQScene(void)
{
}


void MQScene::update(float dt) {
    boost::function<void()> func = UIThreadMQ::getInstance()->getMessage();
    if(func) {
        func();
    }
}