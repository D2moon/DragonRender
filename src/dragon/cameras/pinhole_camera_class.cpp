#include "dragon/core/camera_class.h"
#include <spdlog/spdlog.h>

namespace Dragon {

    inline Transform CalcLookAt(Vector3f p, Vector3f target, Vector3f up) {

//        Vector3f dir = (p - target).normalized();
//        Vector3f left = dir.cross(up).normalized();
//        Vector3f newUp = left.cross(dir).normalized();
        Vector3f dir = (p - target).normalized();
        Vector3f left = up.cross(dir).normalized();
        Vector3f newUp = dir.cross(left).normalized();


        Eigen::Matrix4f result;
        result.coeffRef(0, 0) = left.x();
        result.coeffRef(1, 0) = left.y();
        result.coeffRef(2, 0) = left.z();
        result.coeffRef(3, 0) = 0;
        result.coeffRef(0, 1) = newUp.x();
        result.coeffRef(1, 1) = newUp.y();
        result.coeffRef(2, 1) = newUp.z();
        result.coeffRef(3, 1) = 0;
        result.coeffRef(0, 2) = dir.x();
        result.coeffRef(1, 2) = dir.y();
        result.coeffRef(2, 2) = dir.z();
        result.coeffRef(3, 2) = 0;
        result.coeffRef(0, 3) = p.x();
        result.coeffRef(1, 3) = p.y();
        result.coeffRef(2, 3) = p.z();
        result.coeffRef(3, 3) = 1;
        return Transform(result.inverse());
    }

    inline float DegToRad(float deg) {
        return deg * kPi / 180.f;
    }
    class PinholeCamera : public Camera {
    private:
        float aspect_, scale_;
        float fov_;
    public:
        explicit PinholeCamera(PropertyList properties) {
            output_size_.x() = properties.Get<long long>("width").value_or(800);
            output_size_.y() = properties.Get<long long>("height").value_or(800);
            fov_ = DegToRad(properties.Get<double>("fov").value_or(45.0));
            scale_ = tan(fov_ * 0.5f);
            aspect_ = output_size_.x() / float(output_size_.y());
            inv_output_size_ = Vector2f(1.f / output_size_.x(), 1.f / output_size_.y());


            if(!properties.Get<vector<float>>("matrix").has_value()){
                orig_ = properties.Get<vector<float>>("orig").value();
                target_ = properties.Get<vector<float>>("target").value();
                up_ = properties.Get<vector<float>>("up").value();
                look_at_ = CalcLookAt(orig_, target_, up_);
            }
            else{
                auto vec = properties.Get<vector<float>>("matrix").value();
                look_at_ = Transform(vec);
            }




        }

        void GenerateRay(const Vector2f sample, Ray &ray) const override {

            auto orig = look_at_.inverse() * Point3f(0, 0, 0);

            float x = (sample.x() * inv_output_size_.x() - 0.5f) * 2.f * aspect_ * scale_;
            float y = (sample.y() * inv_output_size_.y() - 0.5f) * 2.f * scale_;

            Vector3f dir(x, y, -1);
            dir = look_at_.inverse() * dir;
            dir.normalize();

            ray = Ray(orig, dir);
        }

    };

    PHOENIX_REGISTER_CLASS(PinholeCamera, "pinhole");
}