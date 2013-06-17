#ifndef __ASIO_PTR_H__
#define __ASIO_PTR_H__

#include <boost/asio.hpp>

namespace sgs{
    namespace net{

        typedef boost::shared_ptr<boost::asio::io_service> IoServicePtr;
        typedef boost::shared_ptr<boost::asio::io_service::work> WorkPtr;
        typedef boost::shared_ptr<boost::asio::ip::tcp::acceptor> AcceptorPtr;

    };
};
#endif 
