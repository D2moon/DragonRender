//
// Created by Dreamoon on 2023/3/21.
//

#ifndef DRAGON_SCENE_H
#define DRAGON_SCENE_H
#include "shape_class.h"
#include "common.h"
#include "tracer_class.h"

namespace Dragon{
    class Scene{
    private:
        map<uint, shared_ptr<Shape>> shapes_dict_;
        Tracer tracer_;
    public:
        void AddShape(shared_ptr<Shape> shape) {
            vector<uint> v = shape->AddToTracer(tracer_);
            for(auto item:v) {
                shapes_dict_[item] = shape;
            }
        }

        Interaction Trace(Ray ray) {
            Interaction interaction;
            interaction.basic = tracer_.TraceRay(ray);
            if(interaction.basic.is_hit) {
                interaction.shape = shapes_dict_.at(interaction.basic.geo_id);
            }
            else {
                interaction.shape = nullptr;
            }
            return interaction;
        }

        void PostProcess(){
            tracer_.FinishAdd();
        }
    };
}
#endif //DRAGON_SCENE_H
