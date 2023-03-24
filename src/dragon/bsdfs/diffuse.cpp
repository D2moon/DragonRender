#include "dragon/core/bsdf_class.h"

namespace Dragon {


    class Diffuse : public Bsdf {
    private:
    public:
        explicit Diffuse(PropertyList properties) {
            base_color_ = properties.Get<vector<float>>("reflectance").value_or(vector<float>{1, 1, 1});

        }

        Color3f Sample(BSDFQueryRecord &rec, float &pdf, const Vector2f &sample) const override {

            rec.wo = SquareToCosineHemisphere(sample);
            rec.wo.normalize();
            pdf = SquareToCosineHemispherePdf(rec.wo);
            if(Frame::CosTheta(rec.wi)<0.f)
                rec.wo.z()*=-1;
            return base_color_;

        };

        Color3f Eval(const BSDFQueryRecord &rec) const override {
//            if (Frame::CosTheta(rec.wo) < 0 || Frame::CosTheta(rec.wi) < 0)
//                return 0.f;
            return kInvPi * base_color_ * abs(Frame::CosTheta(rec.wo));

        }

        float Pdf(const BSDFQueryRecord &rec) const override {
//            if (Frame::CosTheta(rec.wo) < 0 || Frame::CosTheta(rec.wi) < 0)
//                return 0.f;
            Vector3f s = rec.wo;
            s.z() = abs(s.z());
            s.normalize();
            return SquareToCosineHemispherePdf(rec.wo);
        }


    };

    PHOENIX_REGISTER_CLASS(Diffuse, "diffuse");
}