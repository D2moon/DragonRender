#include "dragon/core/integrator.h"

namespace Dragon{
    class NormalColor : public Integrator{
    public:
        NormalColor(PropertyList property){}
        [[nodiscard]] Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray& ray)const override {
            auto hit = scene->Trace(ray);
            if(hit.basic.is_hit){
                auto normal = hit.basic.normal;
                return Color3f(normal.x(), normal.y(), normal.z());
            }
            else {
                return Color3f(0.f, 0.f, 0.f);
            }
        }
    };

    DRAGON_REGISTER_CLASS(NormalColor, "normal");
}