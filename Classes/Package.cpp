#include "Package.h"


namespace sgs {
namespace package {

    PackageHash PackageAdder::packages;

    Package::~Package(void)
    {
    }

    bool Package::init() {
        addGenerals();
        addCards();
        return true;
    }

    void Package::addGenerals() {
    }

    void Package::addCards() {
    }

};
};