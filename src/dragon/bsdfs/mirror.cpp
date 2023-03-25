#include "dragon/core/bsdf_class.h"

namespace Dragon{
    class Mirror : public Bsdf{
    public:
        Mirror(PropertyList property){}

        Color3f Sample(BSDFQueryRecord &rec, float &pdf, const Vector2f &sample) const {
            rec.wo = Vector3f(-rec.wi.x(), -rec.wi.y(), rec.wi.z());
            pdf = 1.0f;
            return Color3f(1.0f);
        }

        Color3f Eval(const BSDFQueryRecord &rec) const {
            auto wo = Vector3f(-rec.wi.x(), -rec.wi.y(), rec.wi.z());
            if(rec.wo.isApprox(wo)) {
                return Color3f(1.0f);
            }
            else {
                return Color3f(0.0f);
            }
        }

        float Pdf(const BSDFQueryRecord &rec) const {
            auto wo = Vector3f(-rec.wi.x(), -rec.wi.y(), rec.wi.z());
            if(rec.wo.isApprox(wo)) {
                return 1.0f;
            }
            else {
                return 0.0f;
            }
        }
    };

    DRAGON_REGISTER_CLASS(Mirror, "mirror");
}