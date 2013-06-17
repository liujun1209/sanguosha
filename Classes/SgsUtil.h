#ifndef __SGS_UTIL_H__
#define __SGS_UTIL_H__

#include "Player.h"

using namespace sgs::core;

namespace sgs {
    namespace utils {

        string getRoleString(Player::Role role);

        Player::Role getRoleFromString(const string& role);

        string getPhaseString(Player::Phase phase);

        Player::Phase getPhaseFromString(const string& phase);

    }
}

#endif