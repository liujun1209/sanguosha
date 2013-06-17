#ifndef __ASIO_SERVER_H__
#define __ASIO_SERVER_H__

#include <string>
#include <map>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/utility.hpp>
#include "MyLock.h"
#include "IoServicePool.h"
#include "ConnectionManager.h"

namespace sgs {
    namespace net {

        class Connection;
        typedef boost::shared_ptr<Connection> ConnectionPtr;

        class IoHandler;
        class UdpIoHandler;
        /**
        * @brief 异步网络框架Server.使用时
        * 需要设置监听端口号和网络事件的处理句柄IoHandler
        * 调用bind方法开始监听端口
        *
        **/
        class AsioServer: public boost::enable_shared_from_this<AsioServer> {
        public:
            AsioServer(void);
            ~AsioServer(void);
            /**
            * @brief 初始化server, 初始化handler，子类可以继承
            *
            * @param port
            */
            void setPort(const short port);

            /**
            * @brief 设置网络事件的处理句柄，
            *
            * @param ioHandlerPtr
            */
            void setIoHandler(boost::shared_ptr<IoHandler> ioHandlerPtr);

            /**
            * @brief 启动监听端口，接受请求
            */
            void bind();

            /**
            * @brief 在指定端口上启动服务并监听
            *
            * @param port
            */
            void bind(const short port);

            void unbind();

            /**
            * @brief 将一个链接放入连接池中holding
            *
            * @param connectionPtr
            */
            void addConnection(ConnectionPtr connectionPtr);

            /**
            * @brief 从连接池中删除指定id的链接
            *
            * @param connectionId
            */
            void delConnection(long connectionId);

            /**
            * @brief 获取一个Connection对象
            *   当connectionId不存在时返回一个空的智能指针。
            *
            * @param connectionId
            *
            * @return 
            */
            ConnectionPtr getConnection(long connectionId);

            /**
            * @brief 向指定的connection id发送message
            *
            * @param connectionId
            * @param msg
            */
            void deliverMsg(const long connectionId, const std::string& msg);

            /**
            * @brief 清除所有保存着的Connection
            */
            void clearAllConnection();


            void startBoardcastLocalIp(const short port);
            void startBoardcastLocalIp();
            void stopBoardcastLocalIp();
            std::string getBrocastMessage() const;

        private :
            short port_;
            //广播端口
            short boardcastPort_;
            //是否开始广播ip
            bool boardcastStarted_;
            //udpsocket
            boost::shared_ptr<boost::asio::ip::udp::socket> udpSocketPtr_;
            //timer
            sgs::utils::Timer boardcastTimer_;
            const static size_t IOSERVER_POOL_SIZE = 8;

            void handleAccept(const boost::system::error_code& e);

            ConnectionPtr newConnection();

            int generateConnectionId();

            boost::shared_ptr<IoHandler> ioHandlerPtr_;

            IoServicePool ioServicePool_;

            boost::asio::ip::tcp::acceptor acceptor_;

            ConnectionPtr newConnection_;

            int connectionCount_;

            /**
            * @brief holding链接管理器
            */
            boost::shared_ptr<ConnectionManager> connectionManagerPtr_;
        };

        /*
        class UdpBroadcaster {
        public:
            
        private:
            
        };
        */
    }
}
#endif
