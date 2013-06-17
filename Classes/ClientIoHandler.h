#ifndef __CLIENT_IO_HANDLER_H__
#define __CLIENT_IO_HANDLER_H__

#include "XmlIoHandler.h"

using namespace sgs::net;

namespace sgs {
    namespace client {

        /**
        * @brief 
        **/
        class ClientIoHandler : public XmlIoHandler
        {
        public:
            ClientIoHandler(void);
            ~ClientIoHandler(void);
            
            virtual void connectionOpened(const boost::shared_ptr<Connection> connectionPtr);
            virtual void messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc);
        };

    }
}


#endif
