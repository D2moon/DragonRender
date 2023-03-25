#pragma once

#include<OpenImageDenoise/oidn.hpp>
#include "bitmap_class.h"
namespace Dragon{

    class Denoiser{
    private:
        oidn::DeviceRef device_;
        oidn::FilterRef filter_,albedo_filter_,normal_filter_;
    public:
        Denoiser();
        Bitmap Denoise(Bitmap color, Bitmap &albedo, Bitmap &normal);

    };

}