#include <iostream>
#include "dragon/core/image_tool.h"
#include "dragon/core/vector_class.h"
#include "dragon/core/common_struct.h"
#include "dragon/core/tracer_class.h"
#include "dragon/core/camera_class.h"
#include <embree3/rtcore.h>
#include "dragon/core/shape_class.h"
#include "dragon/core/scene.h"

#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

using namespace Dragon;


int main()
{
    Scene scene;
    int width=400,height = 400;
    Bitmap img(width,height);

    Vector3f orient(1, 0, 3);

    PropertyList list1;
    list1.Set("width", width);
    list1.Set("height", height);
    auto camera1 = std::dynamic_pointer_cast<Camera>(PhoenixObjectFactory::CreateInstance("pinhole", list1));

//    PropertyList list2;
//    list2.Set("center", vector<float>{0, 0, -5});
//    list2.Set("radius", 2.f);
//    auto sphere1 = std::dynamic_pointer_cast<Shape>(PhoenixObjectFactory::CreateInstance("sphere", list2));
//    scene.AddShape(sphere1);

    PropertyList list3;
    list3.Set("center", vector<float>{1, 1, -5});
    list3.Set("radius", 3.f);
    auto sphere2 = std::dynamic_pointer_cast<Shape>(PhoenixObjectFactory::CreateInstance("sphere", list3));
    scene.AddShape(sphere2);
    scene.PostProcess();

    Color3f color(1, 1, 1);

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            Vector2f pixelSample = Vector2f(i, j);
            Ray ray;
            camera1->GenerateRay(pixelSample, ray);
            auto res = scene.Trace(ray);

            Vector3f lightWay = res.basic.point - orient;
            lightWay.normalize();

            if (res.basic.is_hit) {
                Color3f ambient = 0.1 * color;
                Color3f diffuse = std::max(res.basic.normal.dot(-lightWay), 0.f) * color;

                img.SetColor(i, j, Color3f(ambient + diffuse));

            }
            else {
                img.coeffRef(i, j) = Eigen::Vector3f(0, 0, 0);//与物体没有相交

            }
        }
    }
    write_exr(img,"test.exr");
}