#include "StringResource.h"
#include <boost\lexical_cast.hpp>

namespace sgs {
    namespace utils {

        StringResource * StringResource::instance_ = NULL;

        StringResource::StringResource(void)
        {
            strings_ = CCDictionary::createWithContentsOfFileThreadSafe("strings/strings.xml");
        }

        StringResource * StringResource::getInstance() {
            if(!instance_) {
                instance_ = new StringResource();
            }
            return instance_;
        }

        StringResource::~StringResource(void)
        {
            delete strings_;
        }

        string StringResource::getString(const string& key) {
            return ((CCString*)(strings_->objectForKey(key)))->m_sString;
        }

        string StringResource::getString(const int key) {
            string keyStr;
            try{
                keyStr = boost::lexical_cast<string,int>(key);
            }
            catch(...){}
            return getString(keyStr);
        }
    }
}