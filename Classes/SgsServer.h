#ifndef __SGS_SERVER_H__
#define __SGS_SERVER_H__

#include <boost\smart_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <vector>
#include "AsioServer.h"
#include "XmlIoHandler.h"
#include "ServerPlayer.h"

using namespace std;
using namespace sgs::net;

namespace sgs {
    namespace server {

        enum ServerStatus{Waiting, Gaming};

        //1、等待玩家准备完成
        //3、主公选将
        //4、其他人选将
        //5、
        enum GamingStatus { Ready, LordChoose , OtherChoose , MainLoop };
        /**
        * @brief 逻辑服务器类
        **/
        class SgsServer :public XmlIoHandler
        {
        public:
            SgsServer(void);
            ~SgsServer(void);
            void init();
            /**
            * @brief 启动服务器
            **/
            void start();
            /**
            * @brief 关闭服务器
            **/
            void stop();
            
            void connectionOpened(const boost::shared_ptr<Connection> connectionPtr);
            void messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc);
            void connectionClosed(const boost::shared_ptr<Connection> connectionPtr);
            
            virtual void messageSent(const boost::shared_ptr<Connection> connectionPtr, const std::string& msg);
            

            bool validate(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc);
            boost::shared_ptr<ServerPlayer> getPlayerByConnectionId(long connectionId);
            void sendPacket(boost::shared_ptr<ServerPlayer> player, const string& packet);
            void broadcastPacket(const string& packet);
            void broadcastPacketExclude(boost::shared_ptr<ServerPlayer> player, const string& packet);

            //逻辑相关
            //分配座位与角色
            void assignSeatAndRole();
            //分配主公武将
            void assignLordGeneral();
            //分配其他人武将
            void assignOtherGeneral(const string & lordName);
            //检查是否全部选择完毕
            void checkChooseGeneral();
            //初始洗牌
            int initCards();
            //抓牌
            vector<int> drawCards(int num);
            void discardCards(const set<int>& discardCards);
            
            //主循环
            void mainLoop(bool timerCall = false);

            
        private:
            static const int MAX_PLAYER = 8;
            //主公玩家
            boost::shared_ptr<ServerPlayer> lordPlayer_;
            boost::shared_ptr<ServerPlayer> currentPlayer_;
            //用户列表
            vector<int > playerList_;
            map<int ,boost::shared_ptr<ServerPlayer> > playerMap_;
            //已分配武将
            set<string> alreadyAssignGeneral_;
            //Asio服务器
            boost::shared_ptr<AsioServer> server_;
            //主流程定时器
            Timer mainLoopTimer_;
            //服务器当前状态
            ServerStatus status_;
            //牌堆
            vector<int> cardIdList_;
            //弃牌堆
            vector<int> discardCardIdList_;
            //锁
            boost::shared_mutex mutex_;
            boost::shared_mutex cardMutex_;
            //int playerCount_;
            int readyCount_;
        };

    }
}

#endif