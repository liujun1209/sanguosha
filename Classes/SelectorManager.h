#ifndef __SELECTOR_MANAGER_H__
#define __SELECTOR_MANAGER_H__

#include "CardSelector.h"
#include "PlayerSelector.h"

namespace sgs {
    namespace selector {

        //selector¹ÜÀíÆ÷
        class SelectorManager
        {
        public:
            ~SelectorManager(void);
            static SelectorManager * getInstance();

            CardSelector& getCardSelector();
            PlayerSelector& getPlayerSelector();

        private:
            SelectorManager(void);
            static SelectorManager * instance_;
            CardSelector cardSelector_;
            PlayerSelector playerSelector_;
        };

#define CARD_SELECTOR  SelectorManager::getInstance()->getCardSelector()
#define PLAYER_SELECTOR  SelectorManager::getInstance()->getPlayerSelector();

    }
}

#endif