#include "Structs.h"

namespace sgs {
    namespace core {

        JudgeStruct::JudgeStruct() {
        }

        bool JudgeStruct::isGood() const{
            return false;
        }

        bool JudgeStruct::isGood(const boost::shared_ptr<Card> card) const {
            return false;
        }
        void JudgeStruct::updateResult() {

        }
        bool JudgeStruct::isEffected() {
            return false;
        }
        bool JudgeStruct::isBad() const {
            return false;
        }

        JudgeStructPattern::JudgeStructPattern() {
        }
        //JudgeStructPattern &operator=(const QRegExp &rx);
        //JudgeStructPattern &operator=(const QString &str);
        bool JudgeStructPattern::match(const Player *player, const boost::shared_ptr<Card> card) const {
            return false;
        }
    };
};