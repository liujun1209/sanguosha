#ifndef __STRING_RESOURCE_H__
#define __STRING_RESOURCE_H__

#include <cocos2d.h>
#include <string>

using namespace std;
using namespace cocos2d;

namespace sgs {
    namespace utils {

        class StringResource
        {
        public:
            ~StringResource(void);
            static StringResource * getInstance();
            string getString(const string & key);
            string getString(const int key);
        private:
            static StringResource * instance_;
            StringResource(void);
            CCDictionary *strings_;

        };

        #define STRING_RESOURCE(key) sgs::utils::StringResource::getInstance()->getString(key)
    }
}


#endif