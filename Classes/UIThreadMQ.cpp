#include "UIThreadMQ.h"


UIThreadMQ * UIThreadMQ::instance_ = new UIThreadMQ(); 

UIThreadMQ::UIThreadMQ(void)
{
}


UIThreadMQ::~UIThreadMQ(void)
{
}

UIThreadMQ * UIThreadMQ::getInstance() {
    return instance_;
}

void UIThreadMQ::addMessage(boost::function<void()> func) {
    sgs::utils::ScopedLock lock(mutex_);
    messageQueue_.push(func);
}

boost::function<void()> UIThreadMQ::getMessage() {
    sgs::utils::ScopedLock lock(mutex_);
    if(messageQueue_.size()>0 ) {
        boost::function<void()> func = messageQueue_.front();
        messageQueue_.pop();
        return func;
    }
    return boost::function<void()>();
}

