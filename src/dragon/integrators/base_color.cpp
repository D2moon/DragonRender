#include "dragon/core/integrator.h"
namespace Dragon{
    class BaseColor : public Integrator{
    public:
        BaseColor(PropertyList property){}
        [[nodiscard]] Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray& ray)const override {
            auto hit = scene->Trace(ray);
            if(hit.basic.is_hit) {
                if(hit.hit_type == HitType::Emitter) {
                    return hit.emitter->radiance_;
                }
                else {
                    return hit.shape->bsdf()->base_color_;
                }
            }
            else {
                return Color3f(0.f, 0.f, 0.f);
            }
        }

    };

    DRAGON_REGISTER_CLASS(BaseColor, "base_color");
}