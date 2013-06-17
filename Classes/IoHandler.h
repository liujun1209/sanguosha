#ifndef __IO_HANDLER_H__
#define __IO_HANDLER_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>

namespace sgs{
    namespace net{
        class Connection;

        /**
        * @brief 网络空闲状态定义
        */
        enum IdleStatus{
            /**
            * @brief 读通道空闲
            */
            READER_IDLE = 1,
            /**
            * @brief 写通道空闲
            */
            WRITER_IDLE = 2,
            /**
            * @brief 读写通道均空闲
            */
            BOTH_IDLE = 3
        };

        /**
        * @brief AsioServer异步网络框架的网络事件处理句柄
        *    使用者需要继承该类实现各类I/O事件的处理。
        */
        class IoHandler{
        public :
            IoHandler();

            virtual ~IoHandler();

            /**
            * @brief 链接打开，当有用户请求被AsioServer接受后会触发该方法
            *
            * @param connectionPtr
            */
            //virtual void connectionConnectTimeout(const boost::shared_ptr<Connection> connectionPtr) = 0;
            /**
            * @brief 链接打开，当有用户请求被AsioServer接受后会触发该方法
            *
            * @param connectionPtr
            */
            virtual void connectionOpened(const boost::shared_ptr<Connection> connectionPtr) = 0;

            /**
            * @brief 链接关闭，当执行Connection::close()方法时会触发该方法
            *
            * @param connectionPtr
            */
            virtual void connectionClosed(const boost::shared_ptr<Connection> connectionPtr) = 0;

            /**
            * @brief 链接空闲，当链接的I/O空闲时间超过了指定的最大空闲时间会触发该方法。
            *    成功的读和写操作会更新链接的读写时间。通过Connection::setMaxReadIdleTime和Connection::setMaxWriteIdleTime可以设置链接允许的最大I/O空闲时间
            *
            * @param connectionPtr
            * @param status 链接的空闲状态，
            */
            virtual void connectionIdle(const boost::shared_ptr<Connection> connectionPtr, const IdleStatus status) = 0;

            /**
            * @brief 当链接的异步I/O操作发生异常时会触发该方法，返回发生的异常错误代码
            *
            * @param connectionPtr
            * @param e
            */
            virtual void exceptionCaught(const boost::shared_ptr<Connection> connectionPtr, const boost::system::error_code& e) = 0;

            /**
            * @brief 当链接从I/O读取到内容时会触发该方法，方法返回Connection接收到的内容
            *
            * @param connectionPtr
            * @param message 接收到的内容
            */
            virtual void messageReceived(const boost::shared_ptr<Connection> connectionPtr, const std::string& message) = 0;

            /**
            * @brief 当链接的异步写成功后会触发该方法。
            *   通过调用Connection::write方法可以开始异步写操作
            *
            * @param connectionPtr
            * @param message 写成功的消息内容
            */
            virtual void messageSent(const boost::shared_ptr<Connection> connectionPtr, const std::string& message) = 0;

        };

    };
};

#endif 
