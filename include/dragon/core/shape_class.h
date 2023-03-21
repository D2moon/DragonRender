//
// Created by Dreamoon on 2023/3/21.
//

#ifndef DRAGON_SHAPE_CLASS_H
#define DRAGON_SHAPE_CLASS_H
#include "object_class.h"
#include "vector_class.h"
#include "tracer_class.h"

namespace Dragon{
    class Shape: public DragonObject {
    public:
        PClassType GetClassType() const override {return PClassType::PShape;}
        virtual vector<uint> AddToTracer(Tracer &tracer) = 0;
        virtual void SamplePosition(PositionSampleRecord &pos, Vector2f sample) = 0;
    };
}
#endif //DRAGON_SHAPE_CLASS_H
