#include "XmlIoHandler.h"
#include "Connection.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "MyLogger.h"

using namespace std;
using namespace boost;
using namespace sgs::utils;

namespace sgs{
    namespace net{

        XmlIoHandler::XmlIoHandler(){
        }

        XmlIoHandler::~XmlIoHandler(){
        }

        void XmlIoHandler::connectionOpened(const boost::shared_ptr<Connection> connectionPtr){
            LOG_DEBUG("XmlIoHandler::connectionOpened => cid = %d" , connectionPtr->getId());
            connectionPtr->setMaxReadIdleTime(10);
            connectionPtr->read();
            sgs::tagParser::TagParserPtr parser(new sgs::tagParser::TagParser(boost::shared_ptr<TagHandler>(shared_from_this())));
            id2ParserMap_[connectionPtr->getId()] = parser;
        }

        void XmlIoHandler::connectionClosed(const boost::shared_ptr<Connection> connectionPtr){
            id2ParserMap_.erase(connectionPtr->getId());
            if(connectionPtr->getAttribute("alive") == "true"){
                //MY_INSTANCE(EndpointServer2).acceptRequest("</stream:stream>", connectionPtr);
            }
        }

        void XmlIoHandler::connectionIdle(const boost::shared_ptr<Connection> connectionPtr, const IdleStatus status){
            LOG_DEBUG("XmlIoHandler::connectionIdle => cid = %d,  status = %d" , connectionPtr->getId() , status);

            if(status == READER_IDLE || status == BOTH_IDLE){
                LOG_DEBUG("XmlIoHandler::connectionIdle => check read timeout : cid = %d" , connectionPtr->getId());
                
                connectionPtr->close();
                //MY_INSTANCE(EndpointServer2).delTagparser(connectionPtr->getId());
            }
        }

        void XmlIoHandler::exceptionCaught(const boost::shared_ptr<Connection> connectionPtr, const boost::system::error_code& e){
            LOG_WARN("XmlIoHandler::exceptionCaught => cid = %d, error = %s" , connectionPtr->getId(), e.message().c_str());
            connectionPtr->close();
        }

        void XmlIoHandler::messageReceived(const boost::shared_ptr<net::Connection> connectionPtr, const std::string& message){
            LOG_DEBUG("XmlIoHandler::messageReceived: %s" , message.c_str());
            connectionPtr->setMaxReadIdleTime(90);
            connectionPtr->read();
            id2ParserMap_[connectionPtr->getId()]->feed(connectionPtr, message);
            
        }

        void XmlIoHandler::messaged(const boost::shared_ptr<Connection> connectionPtr, xmlDocumentPtr& doc) {
            
            //LOG_DEBUG("messaged:" << doc);
        }

        void XmlIoHandler::messageSent(const boost::shared_ptr<Connection> connectionPtr, const string& msg){
            
        }

    }
}
