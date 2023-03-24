#include <iostream>
#include "dragon/core/image_tool.h"
#include "dragon/core/vector_class.h"
#include "dragon/core/common_struct.h"
#include "dragon/core/tracer_class.h"
#include "dragon/core/camera_class.h"
#include <embree3/rtcore.h>
#include "dragon/core/shape_class.h"
#include "dragon/core/scene_class.h"
#include "dragon/core/integrator.h"
#include "dragon/core/sampler_class.h"

#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

using namespace Dragon;

template<typename T>
shared_ptr<T> CreateInstance(string name, const PropertyList &proper) {
    return std::dynamic_pointer_cast<T>(PhoenixObjectFactory::CreateInstance(name, proper));
}

int main() {
    auto scene = make_shared<Scene>();
    long long width = 400, height = 400;
    Bitmap img(width, height);

    Vector3f orient(1, 0, 3);

    PropertyList list1;
    list1.Set("width", width);
    list1.Set("height", height);
    list1.Set("orig",vector<float>{0,0,0});
    list1.Set("up",vector<float>{0,1,0});
    list1.Set("target",vector<float>{0,0,-1});
    list1.Set("fov",60.0);

    auto camera1 = std::dynamic_pointer_cast<Camera>(PhoenixObjectFactory::CreateInstance("pinhole", list1));

    PropertyList list2;
    list2.Set("center", vector<float>{-3, -3, -3});
    list2.Set("radius", 1.f);
    auto emitter_shape = std::dynamic_pointer_cast<Shape>(PhoenixObjectFactory::CreateInstance("sphere", list2));

    PropertyList emitter_prop;
    emitter_prop.Set("radiance", vector<float>{1, 1, 1});

    auto emitter = CreateInstance<Emitter>("area", emitter_prop);
    emitter->AddChild(emitter_shape);

    scene->AddEmitter(emitter);

    auto integrator = CreateInstance<Integrator>("base", PropertyList());

    auto sampler = CreateInstance<Sampler>("pcg_random", PropertyList());

    PropertyList diff_prop;
    diff_prop.Set("reflectance", vector<float>{0.5, 0.5, 0});
    auto diffuse = CreateInstance<Bsdf>("dielectric", diff_prop);

    PropertyList list3;
    list3.Set("center", vector<float>{1, 1, -5});
    list3.Set("radius", 2.f);
    auto sphere2 = std::dynamic_pointer_cast<Shape>(PhoenixObjectFactory::CreateInstance("sphere", list3));
    sphere2->AddChild(diffuse);
    scene->AddShape(sphere2);
    scene->PostProcess();


    const int spp = 256;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Color3f color(0);
            for (int k = 0; k < spp; k++) {
                Vector2f pixelSample = Vector2f(i, j) + sampler->Next2D();
                Ray ray;
                camera1->GenerateRay(pixelSample, ray);

                auto res = integrator->Li(scene, sampler, ray);
                color+=res;
            }
            color/=spp;
            img.SetColor(i, j, color);

        }
    }
    write_exr(img, "test.exr");
}