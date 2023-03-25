#include "dragon/core/emitter_class.h"
#include "dragon/core/shape_class.h"
#include "dragon/core/scene_class.h"

namespace Dragon {
    class AreaEmitter : public Emitter {
    private:
        shared_ptr<Shape> shape_;
    public:
        explicit AreaEmitter(PropertyList properties) {
            radiance_ = properties.Get<vector<float>>("radiance").value_or(vector<float>{1, 1, 1});
        }

        void AddChild(shared_ptr<DragonObject> child) override {
            shape_ = std::dynamic_pointer_cast<Shape>(child);
        }

        [[nodiscard]] Color3f Eval(const EmitterQueryRecord &rec) const override {
            float v = rec.n.dot(rec.wi);
//            if (v <= 0.f)
//                return {0, 0, 0};
            return radiance_;
        }

        Color3f Sample(EmitterQueryRecord &rec, Vector2f sample) const override {
            PositionSampleRecord pos_res;
            shape_->SamplePosition(pos_res, sample);

            rec.n = pos_res.normal.normalized();
            rec.p = pos_res.point;
            rec.pdf = pos_res.pdf;
            rec.wi = (rec.ref - rec.p).normalized();
            rec.shadow_ray = Ray(rec.ref, -rec.wi);

            if (rec.n.dot(rec.wi) <= 0) {
                return {0, 0, 0};
            }
            return radiance_;

        }

        void AddToScene(Scene &scene) override {
            auto ids = shape_->AddToTracer(scene.tracer());
            std::for_each(ids.begin(), ids.end(), [&](const auto &item) {
                scene.SetEmitterDict(item, shared_ptr<Emitter>(this));
            });
        }
    };

    DRAGON_REGISTER_CLASS(AreaEmitter, "area");
}