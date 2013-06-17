#ifndef __SGS_CLIENT_H__
#define __SGS_CLIENT_H__

#include <boost\smart_ptr.hpp>
#include "AsioClient.h"
#include "XmlIoHandler.h"
#include <cocos2d.h>

#include "AsioTimer.h"
#include "General.h"

using namespace sgs::utils;

using namespace cocos2d;
using namespace sgs::net;
using namespace sgs::core;

namespace sgs {
    namespace scene {
        class Lobby;
        class WaitingRoom;
        class PlayerTable;
    }

    namespace client {
        class ClientPlayer;
        enum ClientStatus{Waiting, Gaming};

        class SgsClient : public XmlIoHandler
        {
        public:
            SgsClient(bool host = false);
            ~SgsClient(void);
            void init();
            void connect(string ip);
            void disconnect();

            //开始游戏
            void startGame();
            //
            void ready();
            void connectionClosed(const boost::shared_ptr<Connection> connectionPtr);
            void connectionOpened(const boost::shared_ptr<Connection> connectionPtr);
            void messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc);
            
            virtual void messageSent(const boost::shared_ptr<Connection> connectionPtr, const std::string& msg);
            
            void requestRoomInfo();

            void roomInfo(xmlDocumentPtr& doc);
            void playerJoin(xmlDocumentPtr& doc);
            void playerExit(xmlDocumentPtr& doc);

            int getPlayerCount();
            //更新位置与角色
            void updateSeatAndRole(xmlDocumentPtr& doc);
            //游戏中
            void sendPacket(const string& packet);

        private:
            void heartBeat();
            
            bool host_;
            map<int, boost::shared_ptr<ClientPlayer> > playerMap_;
            boost::shared_ptr<AsioClient> client_;
            ClientStatus status_;
            CC_SYNTHESIZE(sgs::scene::WaitingRoom *, waitingRoomPtr_, WaitingRoomPtr);
            CC_SYNTHESIZE(sgs::scene::Lobby *, lobbyPtr_, LobbyPtr);
            CC_SYNTHESIZE(sgs::scene::PlayerTable *, playerTablePtr_, PlayerTablePtr);
            int controlPlayerId_;
            Timer timer_;
        };

    }
}


#endif