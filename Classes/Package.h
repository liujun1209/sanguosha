#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include <map>
#include <vector>
#include <boost\enable_shared_from_this.hpp>
#include <boost\smart_ptr.hpp>
namespace sgs {
namespace core {
    class Player;
}
}
#include "Player.h"
#include "Card.h"
#include "General.h"

using namespace std;
using namespace sgs::core;
using namespace cocos2d;

namespace sgs {
namespace package {

    class CardPattern{
    public:
        virtual bool match(const boost::shared_ptr<Player> player, const boost::shared_ptr<Card> card) const = 0;
        virtual bool willThrow() const{
            return true;
        }
    };

    class Package : public boost::enable_shared_from_this<Package>
    {
    public:
        enum Type{
            GeneralPack,
            CardPack,
            MixedPack,
            SpecialPack,
        };
        CC_SYNTHESIZE(Type, type_, Type);
        //°üÃû×Ö
        CC_SYNTHESIZE(string, name_, Name);
        Package(string name){
            name_ = name;
            type_ = GeneralPack;
        }
        ~Package(void);
        virtual bool init();
        virtual void addGenerals();
        virtual void addCards();

        CC_SYNTHESIZE_PASS_BY_REF(vector<boost::shared_ptr<Card> >, cards_ , Cards);

        CC_SYNTHESIZE_PASS_BY_REF(vector<boost::shared_ptr<General> >, generals_ , Generals);
        
        CC_SYNTHESIZE_PASS_BY_REF(vector<boost::shared_ptr<Skill> >, skills_ , Skills_);
    };
    
    typedef map<string, boost::shared_ptr<Package> > PackageHash;
    class PackageAdder{
    public:
        PackageAdder(const string &name, boost::shared_ptr<Package> pack){
            pack->init();
            packages[name] = pack;
        }
    
        static PackageHash packages;
    };

    #define ADD_PACKAGE(name) static PackageAdder name##PackageAdder(#name, boost::shared_ptr<Package>(new name##Package));
};
};
#endif
