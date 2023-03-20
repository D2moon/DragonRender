#include <iostream>
#include "dragon/core/image_tool.h"
#include "dragon/core/vector_class.h"
#include "dragon/core/common_struct.h"
#include "dragon/core/tracer_class.h"
#include "dragon/core/camera_class.h"
#include <embree3/rtcore.h> // 引入头文件

#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

using namespace Dragon;


int main()
{
    int width=400,height = 400;
    Bitmap img(width,height);
    Vector3f orig(0.f,0.f,0.f);
    Tracer t1;
    t1.AddSphere(Vector3f(0,0,-5),2);//在 (0,0,-5)处放一个半径为2的圆
    t1.FinishAdd();

    PropertyList list1;
    list1.Set("width", width);
    list1.Set("height", height);
    auto camera1 = std::dynamic_pointer_cast<Camera>(PhoenixObjectFactory::CreateInstance("pinhole", list1));
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
//            float x = (float(i)/height)*2.f-1.f;
//            float y = (float(j)/width)*2.f-1.f;//将“照片”映射到（-1，1）
//            Vector3f dir = Vector3f(x,y,-1).normalized();//从原点发出光线，注意这里的方向要归一化
//            Ray ray{orig,dir};

            Vector2f pixelSample = Vector2f(i, j);
            Ray ray;
            camera1->GenerateRay(pixelSample, ray);

            TracerHit res = t1.TraceRay(ray);

            if (res.is_hit) //如果geomID这个变量不等于RTC_INVALID_GEOMETRY_ID，就说明相交了
                img.coeffRef(i, j) = Eigen::Vector3f(1,1,1);
            else
                img.coeffRef(i, j) = Eigen::Vector3f(0, 0, 0);//与物体没有相交
        }
    }
    write_exr(img,"test.exr");
}