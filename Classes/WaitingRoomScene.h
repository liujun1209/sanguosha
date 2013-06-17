#ifndef __WAITING_ROOM_SCENE_H__
#define __WAITING_ROOM_SCENE_H__

#include <boost\smart_ptr.hpp>
#include "SgsServer.h"
#include "SgsClient.h"
#include <cocos2d.h>
#include <cocos-ext.h>
#include <string>
#include <map>

#include "Macros.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

namespace sgs {
    namespace scene {

        /**
        * @brief 用户面板
        **/
        class UserBoardInfo : public cocos2d::CCNode {
        public:
            UserBoardInfo();
            ~UserBoardInfo();
            virtual bool init();
            CREATE_FUNC(UserBoardInfo); 
        private:
            bool ready_;
        };

        /**
        * @brief 座位
        **/
        class Seat : public CCNode {
        public:
            const static int TAG_NAME = 1;
            const static int TAG_MASK = 2;
            const static int TAG_COVER = 3;
            const static int TAG_HEAD = 4;
            CC_SYNTHESIZE_PASS_BY_REF(int , id_, Id);
            CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(host_, Host);
            CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(seated_, Seated);

            Seat(bool host);
            ~Seat();
            bool init();
            static Seat* create(bool host);
            void seat(int id, const string& name);
            void leave();
        };

        /**
        * @brief 等待房间
        **/
        class WaitingRoom : public CCLayer
        {
        public:
            WaitingRoom(bool host);
            ~WaitingRoom(void);

            virtual bool init();

            //开始游戏
            void startGameCallback();

            void playerJoin(int id, const string& name);
            void playerExit(int id);
            
            void startTouchDownAction(CCObject* pSender, CCControlEvent event) ;
            void backTouchDownAction(CCObject* pSender, CCControlEvent event); 

            static WaitingRoom* create(bool host);
            static cocos2d::CCScene* scene(bool host);
        private:
            bool host_;
            CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<sgs::server::SgsServer>, server_, Server);
            CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<sgs::client::SgsClient>, client_, Client);
            
        };


    }
}
#endif