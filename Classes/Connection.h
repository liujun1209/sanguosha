#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string>
#include <queue>
#include <map>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "IoHandler.h"
#include "AsioPtr.h"
#include "MyLock.h"

namespace sgs {
    namespace net {

        class ConnectionManager;

        struct WriteJob{
            std::string message;
        };

        class Connection : public boost::enable_shared_from_this<Connection>{
        public :
            Connection(int cid, IoServicePtr ioServicePtr, boost::shared_ptr<IoHandler> ioHandlerPtr, boost::shared_ptr<ConnectionManager> connectionManagerPtr);

            ~Connection();

            //void connect(std::string ip, short port);

            void open();

            /**
            * @brief 关闭一个Conncetion
            *   关闭完成后会产生IoHandler::connectionClosed调用
            *   参数确认是否立即从ConnectionManager中删除本链接，默认为自动删除
            *
            * @param delFromManager
            */
            void close(bool delFromManager = true);

            /**
            * @brief 获取链接的唯一Id可以通过在AsioServer中设置id来获取带偏移量的id
            *
            * @return 
            */
            int getId();

            /**
            * @brief 获取IoHandler
            *
            * @return 
            */
            boost::shared_ptr<IoHandler> getIoHandler();

            /**
            * @brief 执行异步读取
            *   读取成功后会回调IoHandler::messageReceived方法
            *   出于性能考虑，该方法不是线程安全的
            *   防止出现多线程同时调用该方法。
            */
            void read();

            /**
            * @brief 执行异步写
            *   读取成功后会回调IoHandler::messageSent方法
            *   出于性能考虑，该方法不是线程安全的
            *   防止出现多线程同时调用该方法。
            *
            * @param message
            *
            * @return 
            */
            bool write(const std::string& message);

            /**
            * @brief 获取链接的客户端ip地址
            *
            * @return 
            */
            std::string getRemoteAddress();

            /**
            * @brief 最后一次读或写成功的时间
            *
            * @return 
            */
            time_t getLastIoTime();

            /**
            * @brief 最后一次读成功的时间
            *
            * @return 
            */
            time_t getLastReadTime();

            /**
            * @brief 最后一次写成功的时间
            *
            * @return 
            */
            time_t getLastWriteTime();

            /**
            * @brief 设置属性，用于保存会话相关的变量
            *
            * @param key
            * @param value
            */
            void setAttribute(const std::string& key, const std::string& value);

            /**
            * @brief 设置只有key的属性
            *
            * @param key
            */
            void setAttribute(const std::string& key);

            /**
            * @brief 判断是否包含指定属性名
            *
            * @param key
            *
            * @return 
            */
            bool containsAttribute( const std::string& key);

            /**
            * @brief 获取指定属性，如果属性不存在，返回空字符串
            *
            * @param key
            *
            * @return 
            */
            std::string getAttribute(const std::string& key);

            /**
            * @brief 移除指定属性
            *
            * @param key
            *
            * @return 
            */
            std::string removeAttribute(const std::string& key);

            /**
            * @brief 设置最大读取超时时间，单位秒
            *
            * @param time
            */
            void setMaxReadIdleTime(int time);

            /**
            * @brief 设置最大写超时时间，单位秒
            *
            * @param time
            */
            void setMaxWriteIdleTime(int time);

            boost::asio::ip::tcp::socket& getSocket();

            /**
            * @brief 返回链接是否处于链接状态
            *
            * @return 
            */
            bool isConnected();
        private :

            void handleRead(const boost::system::error_code& e,
                size_t bytes_transferred);

            void handleWrite(const boost::system::error_code& e ,  size_t  len , int i);

            bool asyncWrite(const std::string& message);

            void resetTimer();

            void handleTimer(const IdleStatus idleStatus, const boost::system::error_code& e);

            int id_;

            time_t lastReadTime_;
            time_t lastWriteTime_;

            int maxReadIdleTime_;
            int maxWriteIdleTime_;

            /**
            * @brief 读取内容的次数,失败的读取同样会增加
            */
            long readMessages_;

            /**
            * @brief 写内容的次数,失败的写入同样会增加
            */
            long writtenMessages_;

            bool isReading_;
            bool isWriting_;

            bool isConnected_;

            bool isTimerStarted_;

            boost::asio::ip::tcp::socket socket_;

            boost::asio::deadline_timer timer_;

            boost::array<char, 8192> buffer_;

            boost::shared_ptr<IoHandler> ioHandlerPtr_;
            
            boost::shared_ptr<ConnectionManager> connectionManagerPtr_;
            /**
            * @brief 用于attribute中单独key的保存，做为一个固定的value
            */
            static const std::string ATTRIBUTE_VALUE;

            std::map<std::string, std::string> attributes_;

            std::queue<std::string> writeJobQueue_;

            boost::mutex writeJobQueueMutex_;
            boost::mutex writeMutex_;

            std::string remoteAddress_;
        };

        typedef boost::shared_ptr<Connection> ConnectionPtr;
}
}
#endif