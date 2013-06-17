#ifndef TALK_ENDPOINT_CONNECTION_MANAGER_H_
#define TALK_ENDPOINT_CONNECTION_MANAGER_H_

#include <string>
#include <map>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include "Connection.h"
#include "MyLock.h"
#include "AsioTimer.h"

using namespace sgs::utils;

namespace sgs{
    namespace net{

        typedef boost::shared_ptr<Connection> ConnectionPtr;
        typedef std::map<long, ConnectionPtr> ConnectionPool;

        class ConnectionManager : 
            public boost::enable_shared_from_this<ConnectionManager>,
            private boost::noncopyable{
        public :
            ConnectionManager();

            ConnectionPtr getConnection(long connectionId);

            void addConnection(ConnectionPtr connectionPtr);

            void delConnection(long connectionId);

            void clear();

            void check();

            void start();

            void stop();

        private :
            const static int CONNECTION_POOL_SIZE = 1; //10
            const static int CONNECTION_CHECK_TIME = 10000; //ms

            int getPoolIndex(long connectionId);

            ConnectionPool connectionPools_[CONNECTION_POOL_SIZE];

            boost::shared_mutex connectionPoolMutexs_[CONNECTION_POOL_SIZE];

            Timer timer_;
            //IceUtil::Timer timer_;

            //IceUtil::TimerTaskPtr timerTaskPtr_;

            boost::mutex checkingMutex_;
        };

        typedef boost::shared_ptr<ConnectionManager> ConnectionManagerPtr;

        /**
        * @brief 用户检测超时connection的timer task
        
        class ConnectionManagerCheckTimerTask : public IceUtil::TimerTask {
        public :
            ConnectionManagerCheckTimerTask(ConnectionManagerPtr connectionManagerPtr)
                : managerPtr_(connectionManagerPtr){
            }

            void runTimerTask();
        private :
            ConnectionManagerPtr managerPtr_;
        };
        */

    };
};
#endif 
