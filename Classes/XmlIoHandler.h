#ifndef __SOCKET_IO_HANDLER__
#define __SOCKET_IO_HANDLER__

#include "IoHandler.h"
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <map>
#include "Tag.h"

using namespace std;
using namespace sgs::tagParser;

namespace sgs {
    namespace net{

        class XmlIoHandler : public IoHandler, public TagHandler, public boost::enable_shared_from_this<XmlIoHandler> {

        public:

            XmlIoHandler();

            virtual ~XmlIoHandler();

            virtual void connectionOpened(const boost::shared_ptr<Connection> connectionPtr);

            virtual void connectionClosed(const boost::shared_ptr<Connection> connectionPtr);

            virtual void connectionIdle(const boost::shared_ptr<Connection> connectionPtr, const IdleStatus status);

            virtual void exceptionCaught(const boost::shared_ptr<Connection> connectionPtr, const boost::system::error_code& e);

            void messageReceived(const boost::shared_ptr<Connection> connectionPtr, const std::string& message);

            virtual void messageSent(const boost::shared_ptr<Connection> connectionPtr, const std::string& msg);
            
            virtual void messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc);
            
        private :
            map<long, sgs::tagParser::TagParserPtr> id2ParserMap_;
            //boost::shared_ptr<EndpointServer> endpointServerPtr_;

        };
    }
}
#endif 
