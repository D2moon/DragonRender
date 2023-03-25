#pragma once
#include "dragon/core/common.h"
#include <any>
#include <optional>
namespace Dragon{

    class PropertyList
    {
    private:
        std::map<string,std::any> properties_;

    public:
        template<typename T>
        void Set(string name,T value)
        {
            properties_[name] = value;
        }
        template<typename T>
        std::optional<T> Get(string name)
        {
            if(properties_.count(name))
                return std::any_cast<T>(properties_[name]);
            return std::nullopt;
        }
    };


    class DragonObject {
    public:
        enum class PClassType {
            PCamera,
            PEmitter,
            PIntegrator,
            PSampler,
            PScene,
            PShape,
            PFilm,
            PFilter,
            PBSDF
        };

        virtual void AddChild(shared_ptr<DragonObject> child) {}
        virtual void SetParent(shared_ptr<DragonObject> parent) {}
        virtual void Active() {}

        [[nodiscard]] virtual PClassType GetClassType() const = 0;
    };


    class DragonObjectFactory {
    public:
        typedef std::function<shared_ptr<DragonObject>(const PropertyList&)> Constructor;

        static void MRegisterClass(const std::string& name, const Constructor& constr);

        static shared_ptr<DragonObject> CreateInstance(const std::string& name,
                                                       const PropertyList& propList) {
            //            if (!m_constructors || m_constructors->find(name) == m_constructors->end())
            //                throw NoriException("A constructor for class \"%s\" could not be found!", name);
            return (*constructors_)[name](propList);

        }

    private:
        static std::map<std::string, Constructor>* constructors_;
    };

#define PHOENIX_REGISTER_CLASS(cls, name) \
    shared_ptr<cls> cls ##_create(const PropertyList &list) { \
        return std::make_shared<cls>(list); \
    } \
    static struct cls ##_{ \
        cls ##_() { \
            DragonObjectFactory::MRegisterClass(name, cls ##_create); \
        } \
    } cls ##__PHOENIX_;\

}