#ifndef __UI_THREAD_MQ_H__
#define __UI_THREAD_MQ_H__

#include <boost\function.hpp>
#include <boost\thread.hpp>
#include <queue>

#include "MyLock.h"

using namespace std;

class UIThreadMQ
{
public:
    ~UIThreadMQ(void);
    boost::function<void()> getMessage();
    void addMessage(boost::function<void()>);
    static UIThreadMQ * getInstance();
private:
    static UIThreadMQ * instance_;
    UIThreadMQ(void);
    std::queue<boost::function<void()> > messageQueue_;
    boost::mutex mutex_;
};

#define ADD_MESSAGE(message) UIThreadMQ::getInstance()->addMessage(message)

#endif