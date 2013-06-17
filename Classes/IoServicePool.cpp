#include <boost/thread.hpp>
#include "IoServicePool.h"

using namespace std;

namespace sgs {
    namespace net {

        IoServicePool::IoServicePool(size_t poolSize)
            : nextIndex_(0), size_(poolSize){
                for(size_t i = 0; i < poolSize; ++i){ 
                    IoServicePtr ioServicePtr(new boost::asio::io_service);
                    WorkPtr workPtr(new boost::asio::io_service::work(*ioServicePtr));
                    servicePool_.push_back(ioServicePtr);
                    workPool_.push_back(workPtr);
                }
        }

        void IoServicePool::start(){
            vector<boost::shared_ptr<boost::thread> > threadList;
            for(size_t i = 0; i < size_; ++i){
                boost::shared_ptr<boost::thread> threadPtr(new boost::thread(
                    boost::bind(&boost::asio::io_service::run, servicePool_[i])));
                threadList.push_back(threadPtr);
            }

            for(size_t i = 0; i < threadList.size(); ++i){
                threadList[i]->join();
            }
        }

        void IoServicePool::stop(){
            for(size_t i = 0; i < size_; ++i){
                servicePool_[i]->stop();
            }
        }

        IoServicePtr IoServicePool::getAcceptIoService(){
            return servicePool_[0];
        }

        IoServicePtr IoServicePool::getIoService(){
            IoServicePtr ioServicePtr = servicePool_[nextIndex_];
            if(nextIndex_ != (size_ - 1)){
                ++nextIndex_;
            } else {
                nextIndex_ = 1;
            }
            return ioServicePtr;
        }
    }
}