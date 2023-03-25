#include "dragon/core/integrator.h"
#include "dragon/core/scene_class.h"

namespace Dragon {

    class Base : public Integrator {
    private:
        float russian_ = 0.99;
    public:

        Base(PropertyList properties) {

        }

        Color3f Li(shared_ptr<Scene> scene, shared_ptr<Sampler> sampler, const Ray &ray) const override {
            auto hit = scene->Trace(ray);

            Color3f res(0, 0, 0);
            if (!hit.basic.is_hit)
                return {0, 0, 0};
//            else
//                return {1,1,1};
            if (hit.hit_type == HitType::Emitter) {
                EmitterQueryRecord rec(ray.orig, hit.basic.point, hit.basic.normal);
                return hit.emitter->Eval(rec);
            }
            auto bsdf = hit.shape->bsdf();

            BSDFQueryRecord rec(hit.frame.ToLocal(-ray.dir).normalized());

            float pdf;
            auto color = bsdf->Sample(rec, pdf, sampler->Next2D());
            auto bsdf_v = bsdf->Eval(rec);
            Ray light_ray(hit.basic.point, hit.frame.ToWorld(rec.wo).normalized());

            if (sampler->Next1D() < russian_)
                res += Li(scene, sampler, light_ray).cwiseProduct(color) / russian_;
            if (res.isValid())
                return res;
            return {0, 0, 0};
        }
    };

    DRAGON_REGISTER_CLASS(Base, "base");

}