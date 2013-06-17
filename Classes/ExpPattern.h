#ifndef __EXP_PATTERN_H__
#define __EXP_PATTERN_H__

#include <boost\smart_ptr.hpp>
#include "Package.h"

namespace sgs {
namespace package {

    class ExpPattern : public CardPattern
    {
    public:
        ExpPattern(const string &exp);
        virtual bool match(const boost::shared_ptr<Player> player, const boost::shared_ptr<Card> card) const;
    private:
        string exp;
        bool matchOne(const boost::shared_ptr<Player> player,const boost::shared_ptr<Card> card, string exp) const;
    };


};
};

#endif