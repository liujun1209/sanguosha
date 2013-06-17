#ifndef __PACKET_FACTORY_H__
#define __PACKET_FACTORY_H__

#include <string>
#include <boost\shared_ptr.hpp>
#include "ServerPlayer.h"
#include "ClientPlayer.h"

using namespace std;
using namespace sgs::server;
using namespace sgs::client;

namespace sgs{
    namespace packet {

        
        static const char* PACKET_PLAYER_MESSAGE = "PlayerMessage";
        static const char* PACKET_GAME_START = "GameStart";
        static const char* PACKET_GAME_READY = "GameReady";

        static const char* PACKET_NAME_ERROR = "Error";
        static const char* PACKET_NAME_HEARTBEAT = "HeartBeat";
        static const char* PACKET_NAME_ROOM = "Room";
        static const char* PACKET_NAME_REQUEST_ROOM = "RequestRoom";
        static const char* PACKET_NAME_ROOM_PLAYER_PATH = "/Room/Player";
        static const char* PACKET_NAME_PLAYER = "Player";
        
        static const char* PACKET_NAME_PLAYER_JOIN = "PlayerJoin";
        static const char* PACKET_NAME_PLAYER_EXIT = "PlayerExit";
        static const char* PACKET_NAME_ROOM_DESTORY = "RoomDestory";
        static const char* PACKET_NAME_JOIN_ROOM_SUCCESS = "JoinSuccess";
        static const char* PACKET_NAME_SEAT_AND_ROLE = "AssignSeatAndRole";
        static const char* PACKET_NAME_SEAT_AND_ROLE_PLAYER_PATH = "/AssignSeatAndRole/Player";
        static const char* PACKET_NAME_CHOOSE_LORD = "ChooseLord";
        static const char* PACKET_NAME_CHOOSE_GENERAL = "ChooseGeneral";
        
        static const char* PACKET_NAME_DRAW_CARD = "DrawCard";
        static const char* PACKET_NAME_DISCARD_CARD = "DiscardCard";
        static const char* PACKET_NAME_FORCE_DISCARD_CARD = "ForceDiscardCard";
        static const char* PACKET_NAME_LEFT_PILES = "LeftPiles";
        static const char* PACKET_NAME_CARD = "Card";

        static const char* PACKET_NAME_PHASE_CHANGE = "PhaseChange";
        
        static const char* PACKET_NAME_SHOW_TIME_BAR = "ShowTimeBar";
        static const char* PACKET_NAME_HIDDEN_TIME_BAR = "HiddenTimeBar";
        
        static const char* PACKET_NAME_GENERAL = "General";
        static const char* PACKET_NAME_LORD = "Lord";

        static const char* PACKET_ATTRIBUTE_ID = "id";
        static const char* PACKET_ATTRIBUTE_NAME = "name";
        static const char* PACKET_ATTRIBUTE_PHASE = "phase";
        static const char* PACKET_ATTRIBUTE_NUM = "num";
        static const char* PACKET_ATTRIBUTE_TIME = "time";
        static const char* PACKET_ATTRIBUTE_CODE = "code";
        static const char* PACKET_ATTRIBUTE_HEAD = "head";
        static const char* PACKET_ATTRIBUTE_MSG = "msg";
        static const char* PACKET_ATTRIBUTE_LORD = "lord";
        static const char* PACKET_ATTRIBUTE_LOYALIST = "loyalist";
        static const char* PACKET_ATTRIBUTE_REBEL = "rebel";
        static const char* PACKET_ATTRIBUTE_RENEGADE = "renegade";
        static const char* PACKET_ATTRIBUTE_VERSION = "version";
        static const char* PACKET_ATTRIBUTE_ROLE = "role";

        /**
        * @brife stream开始报文
        **/
        string streamBeginPacket();

        /**
        * @brife stream结束报文
        **/
        string streamEndPacket();
        /**
        * @brife 游戏开始报文
        **/
        string gameStartPacket();

        /**
        * @brife 心跳报文
        **/
        string heartBeatPacket();

        namespace client {
            /**
            * @brife 加入房间报文
            **/
            string joinRoomPacket();

            /**
            * @brife 请求房间信息报文
            **/
            string requestRoomInfoPacket();

            /**
            * @brife
            **/
            string readyPacket();

            /**
            * @brife 选择武将报文
            **/
            string selectGeneralPacket(boost::shared_ptr<Player> player, boost::shared_ptr<General> general);
        }

        namespace server {

            string forceDiscardCards(const boost::shared_ptr<ServerPlayer> player, const set<int>& cards);

            string discardCards(const boost::shared_ptr<ServerPlayer> player, const set<int>& cards);

            /**
            * @brife 更正剩余牌数
            **/
            string leftPilesPacket(const int number);

            /**
            * @brife 显示时间条
            **/
            string showTimeBarPacket(const boost::shared_ptr<ServerPlayer> player, float time);

            /**
            * @brife 隐藏时间条
            **/
            string hiddenTimeBarPacket(const boost::shared_ptr<ServerPlayer> player);

            /**
            * @brife 阶段改变
            **/
            string phaseChangePacket(const boost::shared_ptr<ServerPlayer> player, const Player::Phase phase);
            /**
            * @brife 抓牌
            **/
            string drawCardPacket(const boost::shared_ptr<ServerPlayer> player, const vector<int>& cardIds, bool cardDetail = true);
            
            /**
            * @brife 弃牌
            **/
            string discardCardPacket(const boost::shared_ptr<ServerPlayer> player, const vector<int>& cardIds, bool cardDetail = true);
            
            /**
            * @brife 主公选将
            **/
            string chooseLordGeneral(const boost::shared_ptr<ServerPlayer> player, const vector<string> & generals);
             /**
            * @brife 其他人选将
            **/
            string chooseGeneral(const boost::shared_ptr<ServerPlayer> player, const string & lord, const vector<string> & generals);
            /**
            * @brife 座位与角色信息
            **/
            string seatAndRolePacket(const boost::shared_ptr<ServerPlayer> player, int lord, int loyalist, int rebel, int renegade);
            /**
            * @brife 用户加入房间成功
            **/
            string joinSuccessPacket(const boost::shared_ptr<ServerPlayer> player);

            /**
            * @brife 错误报文
            **/
            string errorMsgPacket(int code ,const string& msg);

            /**
            * @brife 玩家列表报文
            **/
            string playerListPacket(const vector<boost::shared_ptr<ServerPlayer> > playerList);

            /**
            * @brife 新玩家加入报文
            **/
            string newPlayerPacket(const boost::shared_ptr<ServerPlayer> player);

            /**
            * @brife 玩家退出报文
            **/
            string deletePlayerPacket(const boost::shared_ptr<ServerPlayer> player);

            /**
            * @brife 房间解散报文
            **/
            string roomDismissPacket();
        }
    }
}


#endif