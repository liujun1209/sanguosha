#include "Config.h"

#include <cocos2d.h>
#include <fstream>
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>
#include "MyLogger.h"
#include "Constants.h"

using namespace boost;
using namespace cocos2d;

namespace sgs {
    namespace utils {

        Config* Config::instance_ = NULL;

        Config::Config(void)
        {
        }


        Config::~Config(void)
        {
        }

        Config* Config::getInstance() {
            if(!instance_) {
                instance_ = new Config();
                instance_->load();
            }
            return instance_;
        }

        void Config::load() {
            string filePath = getPropertyFile();
            std::ifstream fi(filePath.c_str());
            if(fi) {
                string temp;
                while( fi >> temp ){
                    int pos = temp.find("=");
                    if(pos != string::npos) {
                        string key = temp.substr(0, pos);
                        string value = temp.substr(pos + 1 , temp.length());
                        properties_.insert(pair<string,string>(key, value));
                    }
                }
                fi.close();
            }
            else {
                properties_[CONFIG_KEY_VERSION] = "0.1.0";
                save();
            }
        }
        void Config::save() {
            string filePath = getPropertyFile();
            std::ofstream fo(filePath.c_str());
            if(fo) {
                map<string,string>::iterator it;
                for(it = properties_.begin(); it != properties_.end(); it ++ ){
                    string temp = it->first + "=" + it->second ;
                    fo << temp.c_str() << endl;
                }
                fo.flush();
                fo.close();
            }
        }

        string Config::getPropertyFile() const {
            string path = CCFileUtils::sharedFileUtils()->getWriteablePath();
            string filePath = path + "config.properties";
            LOG_DEBUG(filePath.c_str());
            return filePath;
        }


        string Config::getProperty(const string& key) const{
            map<string,string>::const_iterator it;
            it = properties_.find(key);
            string value;
            if(it != properties_.end()){
                value = (*it).second;
            }
            return value;
        }

        int Config::getPropertyAsInt(const string& key) const{
            map<string,string>::const_iterator it;
            it = properties_.find(key);
            if(it != properties_.end()){
                try{
                    return lexical_cast<int, string>((*it).second);
                }
                catch(...){}
            }
            return 0;
        }

        long Config::getPropertyAsLong(const string& key) const {
            map<string,string>::const_iterator it;
            it = properties_.find(key);
            if(it != properties_.end()){
                try{
                    return lexical_cast<long, string>((*it).second);
                }
                catch(...){}
            }
            return 0l;
        }

        float Config::getPropertyAsFloat(const string& key) const {
            map<string,string>::const_iterator it;
            it = properties_.find(key);
            if(it != properties_.end()){
                try{
                    return lexical_cast<float,string>((*it).second);
                }
                catch(...){}
            }
            return 0.0f;
        }

        bool Config::getPropertyAsBool(const string& key) const{
            map<string,string>::const_iterator it;
            it = properties_.find(key);
            if(it != properties_.end()){
                try{
                    return lexical_cast<bool, string>((*it).second);
                }
                catch(...){}
            }
            return false;
        }

        void Config::setProperty(const string& key, string value) {
            properties_[key] = value;
            save();
        }

        void Config::setProperty(const string& key, int value) {
            string stringValue;
            try{
                stringValue = lexical_cast<string,int>(value);
            }
            catch(...){}
            properties_[key] = stringValue;
            save();
        }

        void Config::setProperty(const string& key, long value) {
            string stringValue;
            try{
                stringValue = lexical_cast<string, long>(value);
            }
            catch(...){}
            properties_[key] = stringValue;
            save();
        }

        void Config::setProperty(const string& key, float value) {
            string stringValue;
            try{
                stringValue = lexical_cast<string, float>(value);
            }
            catch(...){}
            properties_[key] = stringValue;
            save();
        }

        void Config::setProperty(const string& key, bool value) {
            string stringValue;
            try{
                stringValue = lexical_cast<string, bool>(value);
            }
            catch(...){}
            properties_[key] = stringValue;
            save();
        }

        bool Config::containProperty(const string& key) const {
            return properties_.find(key)!= properties_.end();
        }
    }
}