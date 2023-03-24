#pragma once
#include "dragon/core/common.h"
#include "dragon/core/common_struct.h"
#include "dragon/core/object_class.h"

namespace Dragon {

    class Camera: public DragonObject{
    public:
        Vector2i output_size_;
        Vector2f inv_output_size_;
        Vector3f orig_,target_,up_;
        Transform look_at_;
//    public:
//        static Transform CalcLookAt(Vector3f orig,Vector3f target,Vector3f up);
    public:
        Camera()=default;
        Camera(int width,int height):output_size_(width,height),inv_output_size_(1.f/width,1.f/height){}
        virtual void GenerateRay(Vector2f sample,Ray& ray)const =0;
        [[nodiscard]] PClassType GetClassType() const final{return PClassType::PCamera;}

    };
}