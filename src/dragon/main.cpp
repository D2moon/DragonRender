#include <iostream>
#include "dragon/core/image_tool.h"
#include "dragon/core/vector_class.h"
#include <embree3/rtcore.h> // 引入头文件

#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

using namespace Dragon;


// 发生错误时处理错误的函数，此处将错误信息打印出来
void ErrorFunction(void *userPtr, enum RTCError error, const char *str) {
    std::cout<<str<<std::endl;
}

RTCDevice device = rtcNewDevice(nullptr); // device object，用于创建各种相关对象
RTCScene scene = rtcNewScene(device); // scene object ,包含场景中的各个物体
//device = rtcNewDevice(nullptr); // 创建device和scene
//scene = rtcNewScene(device);

void AddSphere(const Eigen::Vector3f &center, float radius) {
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_SPHERE_POINT);// 新建一个 geometry object，用来保存几何体的信息

    auto *vertices = (float *) rtcSetNewGeometryBuffer(geom,
                                                       RTC_BUFFER_TYPE_VERTEX,
                                                       0,
                                                       RTC_FORMAT_FLOAT4,
                                                       4 * sizeof(float),
                                                       1); // 在geometry object 上新建一个存储圆的几何体体信息的buffer。因为圆只需要四个参数就可以表达出来：三位空间中的坐标、半径，因此这里用一个float4来保存
    vertices[0] = static_cast<float>(center.x());
    vertices[1] = static_cast<float>(center.y());
    vertices[2] = static_cast<float>(center.z());
    vertices[3] = static_cast<float>(radius);
    // 填充信息

    rtcCommitGeometry(geom);
    unsigned int id =rtcAttachGeometry(scene, geom); // 这个id是这个圆在场景中的id，因为我们目前就一个圆，暂且不用它
    rtcReleaseGeometry(geom); // 将这个圆添加到场景中


}

struct Ray{
    Eigen::Vector3f orig; //光线的起点
    Eigen::Vector3f dir; //光线的方向
};


RTCRayHit TraceRay(const Ray &ray) {
    struct RTCIntersectContext context{};
    rtcInitIntersectContext(&context); // 新建一个context

    struct RTCRayHit rayhit{}; // 填充查询的结构体
    rayhit.ray.org_x = ray.orig.x();
    rayhit.ray.org_y = ray.orig.y();
    rayhit.ray.org_z = ray.orig.z();
    rayhit.ray.dir_x = ray.dir.x();
    rayhit.ray.dir_y = ray.dir.y();
    rayhit.ray.dir_z = ray.dir.z();
    rayhit.ray.tnear = 1e-5; //限制光线最近的交点位置，目前不太重要
    rayhit.ray.tfar =  1e10; //限制光线最远的交点位置，目前不太重要
    // 这几个参数不需要变，照着抄就行
    rayhit.ray.mask = -1;
    rayhit.ray.flags = 0;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect1(scene, &context, &rayhit);
    return rayhit;
}



int main()
{
    int width=400,height = 400;
    Bitmap img(width,height);
    AddSphere(Vector3f(0,0,-5),2);//在 (0,0,-5)处放一个半径为2的圆
    rtcCommitScene(scene);//更新场景

    Vector3f orig(0.f, 0.f, 0.f);//观察者的位置
    Vector3f light_pos(4, 4, 0);//点光源的位置
    Vector3f color(1, 1, 1);//球的颜色

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float x = (float(i) / height) * 2.f - 1.f;
            float y = (float(j) / width) * 2.f - 1.f;
            Vector3f dir = Vector3f(x, y, -1).normalized();
            Ray ray{orig, dir};

            auto res = TraceRay(ray);


            if (res.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
                Vector3f normal = Vector3f (res.hit.Ng_x, res.hit.Ng_y, res.hit.Ng_z).normalized();// 交点的法线
                Vector3f pos = ray.orig + ray.dir * res.ray.tfar;//交点的位置
                Vector3f light_dir = (light_pos - pos).normalized();//光照方向
                Vector3f view_dir = (orig - pos).normalized();//视角方向
                Vector3f halfway_dir = (light_dir + view_dir).normalized();//半程向量

                Vector3f ambient = 0.05 * color;//环境光

                float diff = std::max(light_dir.dot(normal), 0.f);
                Vector3f diffuse = diff * color; //漫反射

                float spec = std::pow(std::max(normal.dot(halfway_dir), 0.f), 128);
                Vector3f specular = Vector3f(0.3, 0.3, 0.3) * spec;// 镜面反射

                auto res = ambient + diffuse + specular;

                img.coeffRef(i, j) = res;


            } else
                img.coeffRef(i, j) = Eigen::Vector3f(0, 0, 0);
        }
    }
    write_exr(img,"test.exr");
}