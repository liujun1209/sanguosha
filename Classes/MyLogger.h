#ifndef __MY_LOGGER_H__
#define __MY_LOGGER_H__

#include "cocos2d.h"
#include <ostream>
#include <iostream>

namespace sgs {
    namespace utils {

#define LOG_DEBUG(format, ...) cocos2d::CCLog(format, ## __VA_ARGS__)

#define LOG_INFO(format, ...) cocos2d::CCLog(format, ## __VA_ARGS__)

#define LOG_WARN(format, ...) cocos2d::CCLog(format, ## __VA_ARGS__)
#define LOG_ERROR(format, ...) cocos2d::CCLog(format, ## __VA_ARGS__)

    }
}

#endif