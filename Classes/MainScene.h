#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "AsioServer.h"
#include "AsioClient.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CursorTextField.h"


using namespace sgs::ui;

class MainLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(MainLayer);

    /**
    * @brief 显示输入名字对话框
    **/
    void showInputNameDialog();
    void inputName(const string & name);
    void initMenu();
    void clientCallback(const sgs::net::IpAndNameStruct&);
    void modifyString(string name);
    void startServer();
    void startClient();
private:
    boost::shared_ptr<sgs::net::AsioServer> server_;
    sgs::net::AsioClient asioClient;
    CursorTextField *m_pCursorTextField;
    CCMenu* pMenu;
};

#endif  // __HELLOWORLD_SCENE_H__