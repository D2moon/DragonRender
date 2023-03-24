//
// Created by Dreamoon on 2023/3/21.
//

#ifndef DRAGON_SHAPE_CLASS_H
#define DRAGON_SHAPE_CLASS_H
#include "object_class.h"
#include "vector_class.h"
#include "tracer_class.h"
#include "bsdf_class.h"

namespace Dragon{
    class Shape: public DragonObject {
    private:
        shared_ptr<Bsdf> bsdf_;
    public:
        PClassType GetClassType() const override {return PClassType::PShape;}
        virtual shared_ptr<Bsdf> bsdf() { return bsdf_; }
        virtual vector<uint> AddToTracer(Tracer &tracer) = 0;
        virtual void SamplePosition(PositionSampleRecord &pos, Vector2f sample) = 0;

        void AddChild(shared_ptr<DragonObject> child) override {
            bsdf_ = std::dynamic_pointer_cast<Bsdf>(child);
        }
    };
}
#endif //DRAGON_SHAPE_CLASS_H
