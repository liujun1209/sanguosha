#ifndef  __TIMER_H__
#define  __TIMER_H__
#include  <boost/asio.hpp>
#include  <boost/shared_ptr.hpp>
#include  <boost/date_time/posix_time/posix_time.hpp>
#include  <boost/bind.hpp>
#include  <boost/function.hpp>
#include  <map>
#include  <vector>
#include <iostream>
#include <stdio.h>
#include  <boost/thread.hpp>

namespace sgs {
    namespace utils {

        typedef int taskHandle; 
        typedef boost::shared_ptr<boost::asio::deadline_timer>  TimerPtr;


        struct TaskContext{
            taskHandle index_;    /*任务ID*/
            boost::function<void()>func_; /*定时任务主函数对象*/
            TimerPtr timerPtr_;    /*deadline_timer指针*/
            int deadline_time ;    /*延迟时间*/
            boost::asio::strand strand_;    /*保证同一任务只有一个handler被调用*/
            int mode ;//1表示重复，0表示一次
            TaskContext(boost::asio::io_service& ios):strand_(ios){}
        };

        typedef boost::shared_ptr<TaskContext> TaskContextPtr;



        class Timer {

        public:
            /*
            * 定时器构造函数：记录线程数、初始化任务ID初值、初始化strand对象
            */
            Timer(int threadCount):taskIndex_(0),threadCount_(threadCount){}

            /*
            addTask 向定时器添加一个一次性的任务，触发时间为deadline_time秒后;
            return  返回任务句柄
            */
            taskHandle  addTask(boost::function<void ()> func, int deadline_time);

            /*
            addTask 向定时器添加一个重复性的任务，触发时间为每deadline_time秒后;
            */
            taskHandle  addScheduleRepeatedTask(boost::function<void ()> func, int deadline_time);

            /*
            delTask 取消定时任务;
            */
            bool delTask(taskHandle);

             /*
            delTask 取消定时任务;
            */
            bool cleanAllTask();

            /*
            * start 可以认为是 startThreads 的缩写
            * 启动threadCount_个线程，对io_service的run方法进行运行。
            * 注意此调用将阻塞主调线程，需要单独创建线程进行调用
            * */

            void start();

        private:

            /*任务添加接口所调用的内部函数，设置任务主函数、启动的延迟时间以及是否需要重复触发*/

            taskHandle  interAdd(boost::function<void ()> func, int deadline_time, int mode);

            /*deadline_timer的异步调用async_wait的参数列表中参数bind的参数列表中作为参数的回调函数，其包含对回调不成功情况的分支处理逻辑*/

            void  run(TaskContextPtr taskContextPtr, const boost::system::error_code& e);

            /* 保存所有定时任务的map容器 */
            std::map<taskHandle, TaskContextPtr>  taskContextPtrMap;

            /*io_service 和 strand 的声明顺序必须是ios在上、strand在下，因为strand初始化需要依赖ios*/

            boost::asio::io_service ios_; /*deadline_timer所依赖的io_service*/


            /*内部对定时任务进行标识的计数器*/

            int taskIndex_;     

            /*供interadd内部函数使用的确保对进程访问一致性的互斥量*/

            boost::mutex scoped_mutex_;

            /*运行io_service的run方法的线程个数*/

            int threadCount_;

        };

    }
}
#endif 




















