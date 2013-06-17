#include <limits.h>
#include <iostream>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "AsioServer.h"
#include "Connection.h"
#include "Config.h"
#include "Constants.h"
#include "MyLogger.h"


using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

using namespace sgs::utils;

namespace sgs{
    namespace net{

        AsioServer::AsioServer() :
        ioServicePool_(IOSERVER_POOL_SIZE),
        acceptor_(*ioServicePool_.getAcceptIoService()),
        connectionCount_(0),
        connectionManagerPtr_(new ConnectionManager()),
        boardcastTimer_(1),
        boardcastStarted_(false)
    {

        LOG_INFO("AsioServer::AsioServer => Created");
    }

    AsioServer::~AsioServer(){
    }

    void AsioServer::setPort(const short port){
        port_ = port;
    }

    void AsioServer::setIoHandler(boost::shared_ptr<IoHandler> ioHandlerPtr){
        ioHandlerPtr_ = ioHandlerPtr;
    }

    void AsioServer::handleAccept(const boost::system::error_code& e){
        LOG_DEBUG("AsioServer::handleAccept => accept a connection");

        ConnectionPtr acceptConnection = newConnection_;
        newConnection_ = newConnection();
        acceptor_.async_accept(newConnection_->getSocket(),
            boost::bind(&AsioServer::handleAccept, this, 
            boost::asio::placeholders::error));
        LOG_DEBUG("AsioServer::handleAccept => wait a new connection : cid = %d" , newConnection_->getId());
        if(!e){
            try{
                acceptConnection->open();
                LOG_DEBUG("AsioServer::handleAccept => new connection start : cid = %d" , acceptConnection->getId());
            } catch (exception &e){
                LOG_ERROR("AsioServer::handleAccept => new connection start error : what = %s" , e.what());
            }
        } else {
            LOG_ERROR("AsioServer::handleAccept => error : " , e.message().c_str());
        }
    }

    void AsioServer::addConnection(ConnectionPtr connectionPtr){
        connectionManagerPtr_->addConnection(connectionPtr);
    }

    ConnectionPtr AsioServer::getConnection(long connectionId){
        return connectionManagerPtr_->getConnection(connectionId);
    }

    ConnectionPtr AsioServer::newConnection(){
        int cid = generateConnectionId();

        return ConnectionPtr(new Connection(cid, ioServicePool_.getIoService(), ioHandlerPtr_, connectionManagerPtr_));
    }

    inline int AsioServer::generateConnectionId(){
        ++connectionCount_;
        return connectionCount_ ;
    }

    void AsioServer::bind(const short port){
        port_ = port;
        bind();
    }

    void AsioServer::startBoardcastLocalIp(const short port) {
        try{
            udpSocketPtr_ = boost::shared_ptr<boost::asio::ip::udp::socket>(new ip::udp::socket(*ioServicePool_.getAcceptIoService(), ip::udp::endpoint(ip::udp::v4(), port)));
            //boost::asio::socket_base::broadcast option(true);
            //udpSocketPtr_->set_option(option);
            boardcastStarted_ = true;
            while (boardcastStarted_)
            {
                boost::array<char, 1> recv_buf;
                ip::udp::endpoint remote_endpoint;
                boost::system::error_code error;
                // 接收一个字符，这样就得到了远程端点(remote_endpoint)
                udpSocketPtr_->receive_from(boost::asio::buffer(recv_buf),
                    remote_endpoint, 0, error);
                if (error && error != boost::asio::error::message_size)
                    throw boost::system::system_error(error);

                // 向远程端点发送字符串message    
                boost::system::error_code ignored_error;
                udpSocketPtr_->send_to(boost::asio::buffer(getBrocastMessage()),
                    remote_endpoint, 0, ignored_error);
            } 
        }
        catch(exception& e) {
            LOG_ERROR(e.what());
        }
        catch(...) {
            boardcastStarted_ = false;
        }
    }

    void AsioServer::startBoardcastLocalIp() {
        startBoardcastLocalIp(NET_BROADCAST_PORT);
    }

    void AsioServer::stopBoardcastLocalIp() {
        boardcastStarted_ = false;
        if(udpSocketPtr_.get() != NULL) {
            udpSocketPtr_->shutdown(boost::asio::socket_base::shutdown_both);
            udpSocketPtr_->close();
        }
    }

    string AsioServer::getBrocastMessage() const {
        return "<msg><room><name>" + Config::getInstance()->getProperty(CONFIG_KEY_NAME) + "</name></room></msg>";
    }

    void AsioServer::bind(){
        LOG_DEBUG("AsioServer::bind port = %d", port_);
        connectionManagerPtr_->start();
        newConnection_ = newConnection();

        ip::tcp::endpoint endPoint(ip::tcp::v4(), port_);
        acceptor_.open(endPoint.protocol());
        acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endPoint);
        acceptor_.listen(1000);

        acceptor_.async_accept(newConnection_->getSocket(),
            boost::bind(&AsioServer::handleAccept, this, 
            boost::asio::placeholders::error));

        ioServicePool_.start();

        LOG_INFO("AsioServer::bind => finish");
    }

    void AsioServer::unbind(){
        LOG_DEBUG("AsioServer::unbind");
        acceptor_.cancel();
        acceptor_.close();
        ioServicePool_.stop();
        connectionManagerPtr_->stop();
        LOG_INFO("AsioServer::stop => finish");
    }

    void AsioServer::deliverMsg(const long connectionId, const string& msg){
        ConnectionPtr connectionPtr = connectionManagerPtr_->getConnection(connectionId);
        if(!connectionPtr){
            LOG_WARN("AsioServer::deliverMsg => failure ( connection not found ) : cid = %d, msg = %s" , connectionId , msg.c_str());
            return ;
        }
        //LOG_DEBUG("AsioServer::deliverMsg => success : cid = %d, msg = %s" , connectionId , msg.c_str());
    }

    void AsioServer::delConnection(long connectionId){
        connectionManagerPtr_->delConnection(connectionId);
    }

    void AsioServer::clearAllConnection(){
        connectionManagerPtr_->clear();
    }

    };
};
