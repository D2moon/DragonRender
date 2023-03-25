#include "dragon/core/object_class.h"
namespace Dragon
{
    std::map<std::string, DragonObjectFactory::Constructor> *DragonObjectFactory::constructors_ = nullptr;
    void DragonObjectFactory::MRegisterClass(const std::string &name, const Constructor &constr) {
        if (!constructors_)
            constructors_ = new std::map<std::string, DragonObjectFactory::Constructor>();
        (*constructors_)[name] = constr;
    }
}