#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <map>
#include <string>

using namespace std;

namespace sgs {
    namespace utils {

        class Config
        {
        public:
            ~Config(void);
            static Config* getInstance();
            string getProperty(const string& key) const;
            int getPropertyAsInt(const string& key) const;
            long getPropertyAsLong(const string& key) const;
            float getPropertyAsFloat(const string& key) const;
            bool getPropertyAsBool(const string& key) const;
            void setProperty(const string& key, string value);
            void setProperty(const string& key, int value);
            void setProperty(const string& key, long value);
            void setProperty(const string& key, float value);
            void setProperty(const string& key, bool value);
            bool containProperty(const string& key) const;
            void load();
            void save();
            string getPropertyFile() const;
        private:
            static Config* instance_;
            Config(void);
            map<string,string> properties_;
        };

        #define CONFIG_PROPERTY(key) sgs::utils::Config::getInstance()->getProperty(key)

    }
}

#endif