#include "ClientIoHandler.h"
#include "Connection.h"
#include "MyLogger.h"

namespace sgs {
    namespace client {

        ClientIoHandler::ClientIoHandler(void)
        {
        }


        ClientIoHandler::~ClientIoHandler(void)
        {
        }

        void ClientIoHandler::connectionOpened(const boost::shared_ptr<Connection> connectionPtr) {
            connectionPtr->write("<stream:stream>");
            connectionPtr->write("<msg><test/></msg>");
        }
            
        void ClientIoHandler::messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc) {
            stringstream ss;
            ss << doc;
            LOG_DEBUG("client receive: %s" , ss.str().c_str());
        }
    }
}