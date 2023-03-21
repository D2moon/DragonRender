//
// Created by Dreamoon on 2023/3/20.
//

#ifndef DRAGON_COMMON_STRUCT_H
#define DRAGON_COMMON_STRUCT_H

#include "vector_class.h"
#include "common.h"

namespace Dragon{
    class Shape;

    struct TracerHit {
        bool is_hit;
        float t_far;
        Vector2f uv;
        Normal3f normal;
        Point3f point;
        uint geo_id;
        uint prim_id;
    };

    struct Ray {
        Vector3f orig;
        Vector3f dir;
        float t_near;
        float t_far;
        Ray()=default;

        Ray(Vector3f ori, Vector3f di) : orig(std::move(ori)), dir(std::move(di.normalized())) { t_near = 1e-3, t_far = kFInf; }

        Vector3f At(float t) const {
            return orig + dir * t;
        }
    };

    struct Interaction {
        TracerHit basic;
        shared_ptr<Shape> shape;
    };
}
#endif //DRAGON_COMMON_STRUCT_H
