#include "SelectorManager.h"


namespace sgs {
    namespace selector {

        SelectorManager * SelectorManager::instance_ = 0;

        SelectorManager::SelectorManager(void)
        {
        }


        SelectorManager::~SelectorManager(void)
        {
        }

        SelectorManager* SelectorManager::getInstance() {
            if(instance_ == 0) {
                instance_ = new SelectorManager();
            }
            return instance_;
        }

        CardSelector&  SelectorManager::getCardSelector() {
            return cardSelector_;
        }

        PlayerSelector&  SelectorManager::getPlayerSelector() {
            return playerSelector_;
        }
    }
}