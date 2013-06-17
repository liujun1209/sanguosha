#include "AsioClient.h"
#include "ConnectionManager.h"
#include <boost\bind.hpp>
#include "MyLogger.h"
#include "Constants.h"
#include "pugixml.hpp"
#include <boost\algorithm\string.hpp>

using namespace boost;
using namespace sgs::utils;
using namespace boost::asio::ip;

namespace sgs {
    namespace net {

        AsioClient::AsioClient(void):
    ioServicePool_(IOSERVER_POOL_SIZE),
        connectionCount_(0),
        connectionManagerPtr_(new ConnectionManager()),
        timer_(*ioServicePool_.getAcceptIoService()),
        connected_(false)
    {
    }

    ConnectionPtr AsioClient::newConnection(){
        long cid = generateConnectionId();

        return ConnectionPtr(new Connection(cid, ioServicePool_.getIoService(), ioHandlerPtr_, connectionManagerPtr_));
    }

    inline int AsioClient::generateConnectionId(){
        ++connectionCount_;
        return connectionCount_;
    }

    AsioClient::~AsioClient(void)
    {
    }
    ConnectionPtr AsioClient::getConnectionPtr() {
        return connection_;
    }

    void AsioClient::connect(string ip, short port, long timeout) {
        if(!connected_) {
            connection_ = newConnection();
            endpointPtr_ = boost::shared_ptr<ip::tcp::endpoint>(new ip::tcp::endpoint(ip::address::from_string(ip), port)); 
            connection_->getSocket().async_connect(*endpointPtr_, boost::bind(&AsioClient::handleConnect, this, 
                boost::asio::placeholders::error));
            connected_ = true;
            timer_.expires_from_now(boost::posix_time::milliseconds(timeout));
            timer_.async_wait(boost::bind(&AsioClient::connectTimeout, this, boost::asio::placeholders::error));
            ioServicePool_.start();
        }
        else {
            LOG_WARN("AsioClient::connect client has been connected.");
        }
    }

    void AsioClient::disconnect() {
        if(connected_) {
            connection_->close();
            //ioServicePool_.stop();
            connected_ = false;
        }
    }

    void AsioClient::connectTimeout(const boost::system::error_code& err) {
        if(!err) {
            LOG_DEBUG("AsioClient::connectTimeout");
        }
    }

    /**
    * @brief ∑¢ÀÕmessage
    *
    * @param msg
    */
    void AsioClient::deliverMsg(const std::string& msg) {
        if(connected_) {

        }
    }

    void AsioClient::setIoHandler(boost::shared_ptr<IoHandler> ioHandlerPtr){
        ioHandlerPtr_ = ioHandlerPtr;
    }

    void AsioClient::handleConnect(const boost::system::error_code& err) {
        // LOG_DEBUG((e == boost::system::errc::connection_refused));
        timer_.cancel();
        if(!err) {
            try{
                connection_->open();
                LOG_DEBUG("AsioServer::handleAccept => new connection start : cid = %d" , connection_->getId());
            } catch (std::exception &e){
                LOG_ERROR("AsioServer::handleAccept => new connection start error : what = %s" , e.what());
                connected_ = false;
            }
        }
        else {
            connected_ = false;
            LOG_ERROR("AsioClient::handleConnect error:%s", err.message().c_str());
        }

    }

    UdpSearcher::UdpSearcher(): 
    ioServicePool_(IOSERVER_POOL_SIZE), 
        socket_(*ioServicePool_.getAcceptIoService()),
        timer_(*ioServicePool_.getAcceptIoService()){

            socket_.open(udp::v4());
            boost::asio::socket_base::broadcast option(true);
            socket_.set_option(option);
    }

    UdpSearcher::~UdpSearcher() {
    }

    void UdpSearcher::search() {
        try{
            LOG_DEBUG("UdpSearcher::search start search");

            //π„≤•µÿ÷∑
            //LOG_DEBUG("UdpSearcher::search send msg to %s", addr_broadcast.to_string().c_str());
            udp::endpoint destination(address::from_string("255.255.255.255"), NET_BROADCAST_PORT);

            boost::array<char, 1> send_buf   = { 0 };
            socket_.send_to(boost::asio::buffer(send_buf), destination);

            socket_.async_receive_from(boost::asio::buffer(recvBuf),senderEndpoint,0 ,boost::bind(&UdpSearcher::reciveHandle, this, 
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

            timer_.expires_from_now(boost::posix_time::seconds(5));  
            timer_.async_wait(boost::bind(&UdpSearcher::timeoutHandle, this, boost::asio::placeholders::error));  
            ioServicePool_.start();
        }
        catch(std::exception& e) {
            LOG_ERROR("UdpSearcher::search error : %s ", e.what());
        }
    }

    void UdpSearcher::reciveHandle(const boost::system::error_code& err, int length) {
        LOG_DEBUG("UdpSearcher::reciveHandle");
        timer_.cancel();
        if(func_) {
            IpAndNameStruct ipname;
            if(!err) {
                ipname.status = IpAndNameStruct::Recived;
                ipname.ip = senderEndpoint.address().to_string();
                string message(recvBuf.data(), length);
                pugi::xml_document doc;

                doc.load(message.c_str());

                pugi::xml_node node = doc.first_element_by_path("msg/room/name");

                ipname.name = node.child_value();
            }
            else {
                LOG_DEBUG(err.message().c_str());
                ipname.status = IpAndNameStruct::Error;
            }
            func_(ipname);
        }
    }


    void UdpSearcher::timeoutHandle(const boost::system::error_code& err) {
        if(!err){
            socket_.close();
            if(func_) {
                IpAndNameStruct ipname;
                ipname.status = IpAndNameStruct::Timeout;
                func_(ipname);
            }
        }
    }

    void UdpSearcher::setCallback(boost::function<void(const IpAndNameStruct&)> func) {
        func_ = func;
    }

    }
}