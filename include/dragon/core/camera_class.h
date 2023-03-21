#pragma once
#include "dragon/core/common.h"
#include "dragon/core/common_struct.h"
#include "dragon/core/object_class.h"

namespace Dragon {

    class Camera: public DragonObject{
    public:
        Vector2i output_size_;
        Vector2f inv_output_size_;
    public:
        Camera()=default;
        Camera(int width,int height):output_size_(width,height),inv_output_size_(1.f/width,1.f/height){}
        virtual void GenerateRay(Vector2f sample,Ray& ray)=0;
        [[nodiscard]] PClassType GetClassType() const final{return PClassType::PCamera;}

    };
}