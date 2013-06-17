#ifndef __IO_SERVICE_POOL__
#define __IO_SERVICE_POOL__

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "AsioPtr.h"

namespace sgs {
    namespace net {

        class IoServicePool {
        public :
            IoServicePool(size_t poolSize);

            void start();

            void stop();

            IoServicePtr getIoService();
            IoServicePtr getAcceptIoService();
        private :
            std::vector<IoServicePtr> servicePool_;
            std::vector<WorkPtr> workPool_;

            size_t nextIndex_;
            size_t size_;
        };

        typedef boost::shared_ptr<IoServicePool> IoServicePoolPtr;

    }
}

#endif