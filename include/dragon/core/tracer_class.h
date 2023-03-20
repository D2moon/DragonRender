#pragma once
#include <embree3/rtcore.h>
#include "dragon/core/common.h"
#include "dragon/core/common_struct.h"
#include "dragon/core/vector_class.h"

#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

namespace Dragon{
    class Tracer{
    private:
        RTCDevice device_;
        RTCScene scene_;
    public:
        Tracer();
        uint AddSphere(const Vector3f& center,float radius);
        uint AddMesh(const std::vector<Point3f> &_vertices, const std::vector<uint>& indices);
        TracerHit TraceRay (const Ray& ray)const ;
        void FinishAdd();
    };

}