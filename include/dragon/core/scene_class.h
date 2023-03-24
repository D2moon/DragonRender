//
// Created by Dreamoon on 2023/3/21.
//

#ifndef DRAGON_SCENE_CLASS_H
#define DRAGON_SCENE_CLASS_H

#include "shape_class.h"
#include "common.h"
#include "tracer_class.h"
#include "emitter_class.h"

namespace Dragon {
    class Scene {
    private:
        map <uint, shared_ptr<Shape>> shapes_dict_;
        map <uint, shared_ptr<Emitter>> emitter_dict_;
        Tracer tracer_;
    public:
        void AddShape(shared_ptr<Shape> shape) {
            vector<uint> v = shape->AddToTracer(tracer_);
            for (auto item: v) {
                shapes_dict_[item] = shape;
            }
        }

        void AddEmitter(const shared_ptr<Emitter> &emitter) {
            emitter->AddToScene(*this);
        }

        Interaction Trace(Ray ray) {
            TracerHit basic = tracer_.TraceRay(ray);
            basic.normal = basic.normal.normalized();
            Interaction interaction;
            interaction.basic = basic;
            interaction.shape = nullptr;
            interaction.emitter = nullptr;
            bool z = (basic.normal.dot(-ray.dir)) > 0.f;
            if (basic.is_hit) {
                if (shapes_dict_.count(basic.geo_id)) {
                    interaction.hit_type = HitType::Shape;
                    interaction.shape = shapes_dict_.at(basic.geo_id);
                    interaction.frame = Frame(interaction.basic.normal);

                } else {
                    interaction.hit_type = HitType::Emitter;
                    interaction.emitter = emitter_dict_.at(basic.geo_id);
                    interaction.frame = Frame(interaction.basic.normal);
                }
            }
            return interaction;
        }

        void PostProcess() {
            tracer_.FinishAdd();
        }

        Tracer &tracer() {
            return tracer_;
        }

        void SetEmitterDict(uint id, shared_ptr<Emitter> emitter) {
            emitter_dict_[id] = emitter;
        }
    };
}
#endif //DRAGON_SCENE_CLASS_H
