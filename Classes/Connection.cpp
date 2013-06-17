#include <boost/bind.hpp>
#include "Connection.h"
#include "IoHandler.h"
#include "AsioServer.h"
#include  <iostream>
#include "ConnectionManager.h"
#include "MyLogger.h"

using namespace std;
using namespace sgs::utils;

namespace sgs {
    namespace net{

        const string Connection::ATTRIBUTE_VALUE = "";

        Connection::Connection(int cid, IoServicePtr ioServicePtr, boost::shared_ptr<IoHandler> ioHandlerPtr, boost::shared_ptr<ConnectionManager> connectionManagerPtr)
            : id_(cid),
            maxReadIdleTime_(5*60),
            maxWriteIdleTime_(5*60),
            readMessages_(0),
            writtenMessages_(0),
            isReading_(false),
            isWriting_(false),
            isConnected_(false),
            isTimerStarted_(false),
            socket_(*ioServicePtr),
            timer_(*ioServicePtr),
            ioHandlerPtr_(ioHandlerPtr),
            connectionManagerPtr_(connectionManagerPtr){
                LOG_DEBUG("Connection::Connection => Created : cid = %d ", cid);
        }

        Connection::~Connection(){
            LOG_DEBUG("Connection::~Connection => Destroy : cid = %d" , id_);
            close();
        }

        void Connection::open(){
            LOG_DEBUG("Connection::open => begin : cid = %d" , id_);
            socket_.set_option(boost::asio::socket_base::keep_alive(true));
            socket_.set_option(boost::asio::socket_base::reuse_address(true));

            time_t currentTime = std::time(NULL);
            lastReadTime_ = currentTime;
            lastWriteTime_ = currentTime;
            isConnected_ = true;
            try{
                remoteAddress_ = socket_.remote_endpoint().address().to_string();
            } catch (exception &e){
                LOG_ERROR("Connection::open => get remote address error : " , e.what());
            }
            if(connectionManagerPtr_) {
                connectionManagerPtr_->addConnection(shared_from_this());
            }
            ioHandlerPtr_->connectionOpened(shared_from_this());
            if(!isTimerStarted_){
                resetTimer();
            }
        }

        /**
        * 之所以将socket的shutdown和close放在两个try中，
        * 是为了在shutdown出现错误的时候还能有调用close的机会，
        */
        void Connection::close(bool delFromManager){
            if(!isConnected_){
                return;
            }
            isConnected_ = false;
            timer_.cancel();

            try{
                if(!socket_.is_open())
                    return;
                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            } catch (std::exception& e){
                LOG_WARN("Connection::close => shutdown error : cid = %d, , error = %s", id_ ,e.what());
            } catch (...) {
                LOG_WARN("Connection::close => shutdown error : cid = %d, error = unknow " , id_ );
            }

            try{
                socket_.close();
                LOG_DEBUG("Connection::close => success : cid = %d" , id_);
            } catch (std::exception& e){
                LOG_WARN("Connection::close => close error : cid = %d , error = %s" ,id_ , e.what());
            } catch (...) {
                LOG_WARN("Connection::close => close error : cid = %d  , error = unknow " , id_ );
            }

            ioHandlerPtr_->connectionClosed(shared_from_this());
            if(delFromManager && connectionManagerPtr_){
                connectionManagerPtr_->delConnection(id_);
            }
        }

        int Connection::getId(){
            return id_;
        }

        boost::shared_ptr<IoHandler> Connection::getIoHandler(){
            return ioHandlerPtr_;
        }

        string Connection::getRemoteAddress(){
            return remoteAddress_;
        }

        time_t Connection::getLastIoTime(){
            return lastReadTime_ > lastWriteTime_ ? lastReadTime_ : lastWriteTime_;
        }

        time_t Connection::getLastReadTime(){
            return lastReadTime_;
        }

        time_t Connection::getLastWriteTime(){
            return lastWriteTime_;
        }

        void Connection::setAttribute(const string& key, const string& value){
            attributes_[key] = value;
        }

        void Connection::setAttribute(const string& key){
            attributes_[key] = ATTRIBUTE_VALUE;
        }

        bool Connection::containsAttribute(const string& key){
            map<string, string>::iterator it = attributes_.find(key);
            return it != attributes_.end();
        }

        std::string Connection::getAttribute(const string& key){
            map<string, string>::iterator it = attributes_.find(key);
            if(it == attributes_.end()){
                return "";
            } else {
                return it->second;
            }
        }

        std::string Connection::removeAttribute(const string& key){
            map<string, string>::iterator it = attributes_.find(key);
            if(it == attributes_.end()){
                return "";
            } else {
                string value = it->second;
                attributes_.erase(it);
                return value;
            }
        }

        void Connection::read(){
            if(!isConnected_){
                return;
            }

            if(isReading_){
                return;
            } else {
                isReading_ = true;
            }

            //LOG_DEBUG("Connection::read => start async read : cid = " << id_);
            socket_.async_read_some(boost::asio::buffer(buffer_),
                boost::bind(&Connection::handleRead, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

        void Connection::handleRead(const boost::system::error_code& e,
            std::size_t bytes_transferred){

                isReading_ = false;

                if(!e){
                    ++readMessages_;
                    lastReadTime_ = time(NULL);
                    resetTimer();

                    //LOG_DEBUG("Connection::handleRead => read data : cid = " << id_);
                    string message(buffer_.data(), bytes_transferred);
                    ioHandlerPtr_->messageReceived(shared_from_this(), message);

                } else {
                    ioHandlerPtr_->exceptionCaught(shared_from_this(), e);

                    LOG_DEBUG("Connection::handleRead => failure : cid = %d,  , error = %s", id_ , e.message().c_str());
                }
        }

        bool Connection::write(const string& message){
            //LOG_DEBUG("Connection::write => begin = " << id_);

            if(message.empty() /*|| (message == " ")*/){
                return true;
            }

            if(!isConnected_){
                return false;
            }

            // 得到锁，并把队列里的所有消息合并发送
            ScopedLock lock(writeJobQueueMutex_);

            writeJobQueue_.push(message);

            // 正在写，直接返回，数据已经在队列中，
            // 本次写完成后将被调度
            if(isWriting_){
                return true;
            }else{
                isWriting_ = true ;
            }

            return asyncWrite(writeJobQueue_.front());

        }

        bool Connection::asyncWrite(const string& message){

            if(socket_.is_open()){
                boost::asio::async_write(socket_, boost::asio::buffer(message), 
                    boost::bind(&Connection::handleWrite, shared_from_this(),
                    boost::asio::placeholders::error , boost::asio::placeholders::bytes_transferred , message.size()));
                return true;
            } else {
                LOG_WARN("Connection::write => failure ( socket is close ) : cid = %d" , id_);
                return false;
            }
        }

        void Connection::handleWrite(const boost::system::error_code& e ,  size_t  len , int i){


            ++writtenMessages_;
            lastWriteTime_ = time(NULL);
            resetTimer();
            if(!e){
                ioHandlerPtr_->messageSent(shared_from_this(), writeJobQueue_.front());
            } else {
                ioHandlerPtr_->exceptionCaught(shared_from_this(), e);
                LOG_WARN("Connection::handleWrite => sysWrite failure : cid = %d, error = %s" , id_ ,e.message().c_str());
            }

            // 检查是否还有待发送消息，如果有合并发送直到队列为空
            ScopedLock lock(writeJobQueueMutex_);

            // 弹出队首（已经发送完毕）
            if(writeJobQueue_.empty()){
                isWriting_ = false;
                return ;
            }
            writeJobQueue_.pop();

            if (!writeJobQueue_.empty()){
                asyncWrite(writeJobQueue_.front());
            } else {
                isWriting_=false;
            }
        }

        void Connection::setMaxReadIdleTime(int time){
            maxReadIdleTime_ = time;
            resetTimer();
            //LOG_DEBUG("Connection::setMaxReadIdleTime => resetTimer : cid = " << id_);
        }

        void Connection::setMaxWriteIdleTime(int time){
            maxWriteIdleTime_ = time;
            resetTimer();
            //LOG_DEBUG("Connection::setMaxWriteIdleTime => resetTimer : cid = " << id_);
        }

        boost::asio::ip::tcp::socket& Connection::getSocket(){
            return socket_;
        }

        bool Connection::isConnected(){
            return isConnected_;
        }

        void Connection::resetTimer(){
            if(!isConnected_){
                return;
            }
            if(!isTimerStarted_){
                isTimerStarted_ = true;
            }
            int now = time(NULL);
            int readSpare = lastReadTime_ + (long) maxReadIdleTime_ - now;
            int writeSpare = lastWriteTime_ + (long) maxWriteIdleTime_ - now;

            int nextTime = readSpare > writeSpare ? writeSpare : readSpare;

            if(timer_.expires_from_now(boost::posix_time::seconds(nextTime)) > 0){
                //LOG_DEBUG("Connection::resetTimer => cancel and set timer(" << nextTime << ") : cid = " << id_);
            } else {
                //LOG_DEBUG("Connection::resetTimer => set timer(" << nextTime << ") : cid = " << id_);
            }

            IdleStatus idleStatus;
            if(readSpare > writeSpare){
                idleStatus = WRITER_IDLE;
            } else if (writeSpare > readSpare) {
                idleStatus = READER_IDLE;
            } else {
                idleStatus = BOTH_IDLE;
            }

            timer_.async_wait(boost::bind(&Connection::handleTimer, shared_from_this(), idleStatus, _1));
            return;
        }

        void Connection::handleTimer(const IdleStatus idleStatus, const boost::system::error_code& e){
            if(e != boost::asio::error::operation_aborted){
                if(!e){
                    if(!isConnected_){
                        return;
                    }
                    //LOG_DEBUG("Connection::handleTimer => idle : id = " << id_ << " status = " << idleStatus);

                    ioHandlerPtr_->connectionIdle(shared_from_this(), idleStatus);
                } else {
                    LOG_WARN("Connection::handleTimer => get an error : %s" , e.message().c_str());
                }
            } else {
                //LOG_DEBUG("Connection::handleTimer => timer has canceled");
            }
        }

    };
};
