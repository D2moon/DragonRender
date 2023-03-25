//
// Created by Dreamoon on 2023/3/21.
//

#ifndef DRAGON_EMITTER_H
#define DRAGON_EMITTER_H

#include "object_class.h"
#include "vector_class.h"
#include "common_struct.h"

namespace Dragon{
    struct EmitterQueryRecord {
        Vector3f ref;       // 物体交点
        Vector3f p;         // 光源交点
        Vector3f n;         // 物体交点法线
        Vector3f wi;        // 光线向量
        float pdf;          // 光线概率
        Ray shadow_ray;     // 。。。

        EmitterQueryRecord(const Vector3f &ref, const Vector3f &p, Vector3f n) :
                ref(ref), p(p), n(std::move(n)) {
            pdf = .0f;
            wi = (ref - p).normalized();
        }

        EmitterQueryRecord(const Vector3f &re) : ref(re) {}
    };
    class Scene;
    class Emitter : public DragonObject{
    public:

        Color3f radiance_;

        PClassType GetClassType() const override {return PClassType::PEmitter;}

        [[nodiscard]] virtual Color3f Eval(const EmitterQueryRecord &record) const = 0;

        virtual Color3f Sample(EmitterQueryRecord &rec, Vector2f sample) const = 0;

        virtual void AddToScene(Scene& scene) = 0;
    };
}
#endif //DRAGON_EMITTER_H
