#include <iostream>
#include "dragon/core/image_tool.h"
#include "dragon/core/vector_class.h"
#include "dragon/core/common_struct.h"
#include "dragon/core/camera_class.h"
#include <embree3/rtcore.h>
#include "dragon/core/shape_class.h"
#include "dragon/core/scene_class.h"
#include "dragon/core/integrator.h"
#include "dragon/core/sampler_class.h"
#include "dragon/core/denoiser_class.h"
#include "ext/indicators.h"

#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

using namespace Dragon;
using namespace indicators;

template<typename T>
shared_ptr<T> CreateInstance(string name, const PropertyList &proper) {
    return std::dynamic_pointer_cast<T>(DragonObjectFactory::CreateInstance(name, proper));
}

void addFile(std::string file_name, shared_ptr<Scene> scene, shared_ptr<Bsdf> diffuse)
{
    PropertyList file_prop;
    file_prop.Set<string>("filename",file_name);
    auto ceil_shape = CreateInstance<Shape>("obj",file_prop);
    ceil_shape->AddChild(diffuse);
    scene->AddShape(ceil_shape);
}

int main() {


    indicators::ProgressBar render_bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{" ["},
            indicators::option::Fill{"="},
            indicators::option::Lead{"="},
            indicators::option::Remainder{"-"},
            indicators::option::End{"]"},
            indicators::option::PrefixText{"Rendering"},
            indicators::option::ForegroundColor{indicators::Color::yellow},
            indicators::option::ShowElapsedTime{true},
            indicators::option::ShowRemainingTime{true},
            indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
    };


    auto scene = make_shared<Scene>();
    long long width = 1080, height = 1080;
    Bitmap img(width, height);

    PropertyList list1;
    list1.Set("width", width);
    list1.Set("height", height);
    list1.Set("orig",vector<float>{0,0,4});
    list1.Set("up",vector<float>{0,1,0});
    list1.Set("target",vector<float>{0,0,0});
    list1.Set("fov",39.3077);

    auto camera = std::dynamic_pointer_cast<Camera>(DragonObjectFactory::CreateInstance("pinhole", list1));
    auto integrator = CreateInstance<Integrator>("base", PropertyList());
    auto sampler = CreateInstance<Sampler>("pcg_random", PropertyList());
    PropertyList diff_prop;
    diff_prop.Set("reflectance", vector<float>{0.105421, 0.37798, 0.076425});
    auto green = CreateInstance<Bsdf>("diffuse", diff_prop);

    diff_prop.Set("reflectance", vector<float>{0.570068, 0.0430135, 0.0443706});
    auto red = CreateInstance<Bsdf>("diffuse", diff_prop);

    diff_prop.Set("reflectance", vector<float>{0.885809, 0.698859, 0.666422});
    auto white = CreateInstance<Bsdf>("diffuse", diff_prop);

    diff_prop.Set("reflectance", vector<float>{0.85, 0.85, 0.85});
    auto gray = CreateInstance<Bsdf>("diffuse", diff_prop);

    auto dielectric = CreateInstance<Bsdf>("dielectric",diff_prop);
    auto mirror = CreateInstance<Bsdf>("mirror", diff_prop);


    PropertyList file_prop;
    file_prop.Set<string>("filename","meshes/cbox_luminaire.obj");
    auto lumi_shape = CreateInstance<Shape>("obj",file_prop);

    PropertyList emitter_prop;
    emitter_prop.Set("radiance", vector<float>{18.387, 13.9873, 6.75357});

    auto emitter = CreateInstance<Emitter>("area",emitter_prop);
    emitter->AddChild(lumi_shape);
    scene->AddEmitter(emitter);

    addFile("meshes/cbox_back.obj", scene, white);

    addFile("meshes/cbox_ceiling.obj", scene, white);
    addFile("meshes/cbox_floor.obj", scene, white);
    addFile("meshes/cbox_redwall.obj", scene, red);
    addFile("meshes/cbox_greenwall.obj", scene, green);
    addFile("meshes/dragonobj.obj",scene,dielectric);


    PropertyList list3;
    list3.Set("center", vector<float>{0, 0, 0});
    list3.Set("radius", 0.25f);
    auto sphere2 = std::dynamic_pointer_cast<Shape>(DragonObjectFactory::CreateInstance("sphere", list3));
    sphere2->AddChild(mirror);
    scene->AddShape(sphere2);
//
//    list3.Set("center", vector<float>{0.5, -0.75, -0.2});
//    list3.Set("radius", 0.25f);
//    auto sphere3 = std::dynamic_pointer_cast<Shape>(DragonObjectFactory::CreateInstance("sphere", list3));
//    sphere3->AddChild(gray);
//    scene->AddShape(sphere3);
//
//    list3.Set("center", vector<float>{0.25, 0.0, -0.2});
//    list3.Set("radius", 0.25f);
//    auto sphere4 = std::dynamic_pointer_cast<Shape>(DragonObjectFactory::CreateInstance("sphere", list3));
//    sphere4->AddChild(mirror);
//    scene->AddShape(sphere4);

    scene->PostProcess();


    const int spp = 256;
    const int all = width*height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            render_bar.set_progress(((i*height+j)*100)/all);
            Color3f color(0);
            for (int k = 0; k < spp; k++) {
                Vector2f pixelSample = Vector2f(i, j) + sampler->Next2D();
                Ray ray;
                camera->GenerateRay(pixelSample, ray);

                auto res = integrator->Li(scene, sampler, ray);
                color+=res;
            }
            color/=spp;
            img.SetColor(i, j, color);

        }
    }

    auto base_color = CreateInstance<Integrator>("base_color", PropertyList());
    auto normal_color = CreateInstance<Integrator>("normal", PropertyList());
    Bitmap base_img(width, height);
    Bitmap normal_img(width, height);

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            Vector2f pixel = Vector2f(i, j);
            Ray ray;
            camera->GenerateRay(pixel, ray);
            auto co1 = base_color->Li(scene, sampler, ray);
            auto co2 = normal_color->Li(scene, sampler, ray);
            base_img.SetColor(i, j, co1);
            normal_img.SetColor(i, j, co2);
        }
    }
    Denoiser denoiser;
    auto res = denoiser.Denoise(img, base_img, normal_img);
    write_exr(img, "test.exr");
    write_exr(base_img, "base_img.exr");
    write_exr(normal_img, "normal_img.exr");
    write_exr(res, "res.exr");

}