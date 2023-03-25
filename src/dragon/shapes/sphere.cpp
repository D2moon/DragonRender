//
// Created by Dreamoon on 2023/3/21.
//
#include "dragon/core/shape_class.h"
#include "dragon/core/sampler_class.h"

namespace Dragon{
    class Sphere : public Shape{
    private:
        Vector3f center_;
        float radius_;

    public:
        Sphere(PropertyList properties){
            center_ = properties.Get<vector<float>>("center").value_or(vector<float>{0, 0, 0});
            radius_ = properties.Get<float>("radius").value_or(1);
            area_ = 4*kPi*radius_*radius_;
            inv_area_ = 1.f/area_;
        }

        vector<uint> AddToTracer(Tracer &tracer) override {
            return {tracer.AddSphere(center_, radius_)};
        }

        void SamplePosition(PositionSampleRecord &rec, Vector2f sample) override {
            auto v = SquareToUniformSphere(sample).normalized();
            rec.point = center_ + radius_ * v;
            rec.normal = v;
            rec.pdf = inv_area_;
        }
    };

    PHOENIX_REGISTER_CLASS(Sphere,"sphere");
}
