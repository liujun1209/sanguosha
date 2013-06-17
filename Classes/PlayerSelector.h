#ifndef __PLAYER_SELECTOR_H__
#define __PLAYER_SELECTOR_H__

#include "ClientPlayer.h"

using namespace sgs::client;

namespace sgs {
    namespace selector {

        //Íæ¼ÒÑ¡ÔñÆ÷
        class PlayerSelector
        {
        public:
            PlayerSelector(void);
            ~PlayerSelector(void);

        private:
            boost::shared_ptr<ClientPlayer> controllPlayer;
            
        };


    }
}

#endif