#ifndef __ASIO_CLIENT_H__
#define __ASIO_CLIENT_H__

#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Connection.h"
#include "IoServicePool.h"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace sgs {
    namespace net {

        class IoHandler;
        
        typedef boost::shared_ptr<ip::tcp::socket> SocketPtr;

        class AsioClient: public boost::enable_shared_from_this<AsioClient> {
        public:
            AsioClient(void);
            ~AsioClient(void);
            void connect(string ip, short port, long timeout = 2000);
            void disconnect();

            ConnectionPtr getConnectionPtr();
            /**
            * @brief 发送message
            *
            * @param msg
            */
            void deliverMsg(const std::string& msg);
            
            /**
            * @brief 设置网络事件的处理句柄，
            *
            * @param ioHandlerPtr
            */
            void setIoHandler(boost::shared_ptr<IoHandler> ioHandlerPtr);

        private:
            const static size_t IOSERVER_POOL_SIZE = 2;
            void connectTimeout(const boost::system::error_code& err);
            void handleConnect(const boost::system::error_code& e);
            
            ConnectionPtr newConnection();

            int generateConnectionId();
            
            IoServicePool ioServicePool_;

            int connectionCount_;
            //
            boost::shared_ptr<ip::tcp::endpoint> endpointPtr_;

            deadline_timer timer_;  
            
            ConnectionPtr connection_;
            //
            boost::shared_ptr<IoHandler> ioHandlerPtr_;
            //
            /**
            * @brief holding链接管理器
            */
            boost::shared_ptr<ConnectionManager> connectionManagerPtr_;

            bool connected_;
        };

        typedef struct {
            enum Status{ Recived, Timeout , Error};
            Status status;
            string ip;
            string name;
        } IpAndNameStruct;

        class UdpSearcher {
        public :
            UdpSearcher();
            ~UdpSearcher();
            
            void search();
            void setCallback(boost::function<void(const IpAndNameStruct&)> func);
        private:
             void reciveHandle(const boost::system::error_code& err, int length);
            void timeoutHandle(const boost::system::error_code& err);
            const static size_t IOSERVER_POOL_SIZE = 1;
            IoServicePool ioServicePool_;  
            deadline_timer timer_;  
            boost::asio::ip::udp::socket socket_;  
            boost::asio::ip::udp::endpoint senderEndpoint;
            boost::array<char, 128> recvBuf;
            
            boost::function<void(const IpAndNameStruct&)> func_;
        };

    }
}

#endif