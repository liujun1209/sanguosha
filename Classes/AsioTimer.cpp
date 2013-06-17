#include "AsioTimer.h"


using namespace  std;
using namespace  boost::asio;
using namespace  boost;


namespace sgs {
    namespace utils {

        void Timer::run(TaskContextPtr taskContextPtr, const boost::system::error_code& e){
            if(e == boost::asio::error::operation_aborted){
                return ;
            }
            taskContextPtr->func_();
            if(taskContextPtr->mode == 1){
                TimerPtr  timerPtr = taskContextPtr->timerPtr_;
                timerPtr->expires_at(timerPtr->expires_at() + boost::posix_time::millisec(taskContextPtr->deadline_time));
                timerPtr->async_wait(taskContextPtr->strand_.wrap(boost::bind(&Timer::run, this, taskContextPtr, boost::asio::placeholders::error)));
            }else{

            }
        }


        taskHandle  Timer::addTask(boost::function<void ()> func, int deadline_time){
            return   interAdd(func, deadline_time, 0);
        }

        taskHandle  Timer::addScheduleRepeatedTask(boost::function<void ()> func, int deadline_time){
            return   interAdd(func, deadline_time, 1);
        }


        taskHandle  Timer::interAdd(boost::function<void ()> func, int deadline_time , int mode){
            mutex::scoped_lock lock(scoped_mutex_);
            int  index = taskIndex_++;
            boost::shared_ptr<deadline_timer> timerPtr(new deadline_timer(ios_,posix_time::millisec(deadline_time)));
            TaskContextPtr  taskContextPtr(new TaskContext(ios_));
            taskContextPtr->func_ = func;
            taskContextPtr->index_ = index;
            if(mode == 1){
                taskContextPtr->mode = 1 ;
            }else{
                taskContextPtr->mode = 0 ;
            }
            taskContextPtr->timerPtr_ = timerPtr;
            taskContextPtr->deadline_time = deadline_time;
            taskContextPtrMap.insert(std::map<taskHandle,TaskContextPtr>::value_type(index,taskContextPtr));
            timerPtr->async_wait(taskContextPtr->strand_.wrap(boost::bind(&Timer::run, this, taskContextPtr, boost::asio::placeholders::error)));
            return index;
        }


        void Timer::start(){
            size_t size_ = threadCount_ ;
            vector<boost::shared_ptr<boost::thread> > threadList;

            for(size_t i = 0; i < size_; ++i){
                boost::shared_ptr<boost::thread> threadPtr(new boost::thread(
                    boost::bind(&boost::asio::io_service::run, &ios_)));
                threadList.push_back(threadPtr);
            }


            for(size_t i = 0; i < threadList.size(); ++i){
                threadList[i]->join();
            }
            std::cout<<"Timer out of work"<<std::endl;

        }

        bool Timer::delTask(taskHandle index){
            mutex::scoped_lock lock(scoped_mutex_);
            std::map<taskHandle,TaskContextPtr>::iterator it = taskContextPtrMap.find(index);
            if(it != taskContextPtrMap.end()){
                it->second->timerPtr_->cancel();
                taskContextPtrMap.erase(it);
            }
            return 1;
        }

        bool Timer::cleanAllTask() {
            mutex::scoped_lock lock(scoped_mutex_);
            std::map<taskHandle,TaskContextPtr>::iterator it;
            for(it = taskContextPtrMap.begin(); it != taskContextPtrMap.end(); it++) {
                it->second->timerPtr_->cancel(); 
            }
            taskContextPtrMap.clear();
            return 1;
        }


    }
}