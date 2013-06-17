#include "SgsUtil.h"

namespace sgs {
    namespace utils {

        Player::Phase getPhaseFromString(const string& phase) {
            if(phase == "start") {
                return Player::Start;
            }
            else if(phase == "judge") {
                return Player::Judge;
            }
            else if(phase == "draw") {
                return Player::Draw;
            }
            else if(phase == "play") {
                return Player::Play;
            }
            else if(phase == "discard") {
                return Player::Discard;
            }
            else if(phase == "finish") {
                return Player::Finish;
            }
            else {
                return Player::NotActive;
            }
        }

        string getPhaseString(Player::Phase phase) {
            switch(phase) {
            case Player::Start:
                return "start";
                break;
            case Player::Judge:
                return "judge";
                break;
            case Player::Draw:
                return "draw";
                break;
            case Player::Play:
                return "play";
                break;
            case Player::Discard:
                return "discard";
                break;
            case Player::Finish:
                return "finish";
                break;
            default:
                return "unknow";
                break;
            }
        }

        Player::Role getRoleFromString(const string& role) {
            if(role == "lord") {
                return Player::Lord;
            }
            else if(role == "loyalist") {
                return Player::Loyalist;
            }
            else if(role == "rebel") {
                return Player::Rebel;
            }
            else if(role == "renegade") {
                return Player::Renegade;
            }
            else {
                return Player::Unknow;
            }
        }

        string getRoleString(Player::Role role) {
            switch(role) {
            case Player::Lord:
                return "lord";
                break;
            case Player::Loyalist:
                return "loyalist";
                break;
            case Player::Rebel:
                return "rebel";
                break;
            case Player::Renegade:
                return "renegade";
                break;
            default:
                return "unknow";
                break;
            }
        }

    }
}

