#include "dragon/core/object_class.h"
namespace Dragon
{
    std::map<std::string, PhoenixObjectFactory::Constructor> *PhoenixObjectFactory::constructors_ = nullptr;
    void PhoenixObjectFactory::MRegisterClass(const std::string &name, const Constructor &constr) {
        if (!constructors_)
            constructors_ = new std::map<std::string, PhoenixObjectFactory::Constructor>();
        (*constructors_)[name] = constr;
    }
}