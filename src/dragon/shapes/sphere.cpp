//
// Created by Dreamoon on 2023/3/21.
//
#include "dragon/core/shape_class.h"

namespace Dragon{
    class Sphere : public Shape{
    private:
        Vector3f center_;
        float radius_;

    public:
        Sphere(PropertyList properties){
            center_ = properties.Get<vector<float>>("center").value_or(vector<float>{0, 0, 0});
            radius_ = properties.Get<float>("radius").value_or(1);
        }

        vector<uint> AddToTracer(Tracer &tracer) override {
            return {tracer.AddSphere(center_, radius_)};
        }
    };

    PHOENIX_REGISTER_CLASS(Sphere,"sphere");
}
