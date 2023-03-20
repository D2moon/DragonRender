#include <iostream>
#include "dragon/core/bitmap_class.h"
#define TINYEXR_USE_MINIZ 0
#include <zlib.h>
#define TINYEXR_IMPLEMENTATION
#include "ext/tinyexr.h"

using namespace Dragon;

void write_exr(const Bitmap &bitmap, std::string filename) {
    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    int width = bitmap.width();
    int height = bitmap.height();
    auto rgb = reinterpret_cast<const float*>(bitmap.data());

    image.num_channels = 3;

    std::vector<float> images[3];
    images[0].resize(width * height);
    images[1].resize(width * height);
    images[2].resize(width * height);

    // Split RGBRGBRGB... into R, G and B layer
    for (int i = 0; i < width * height; i++) {
        images[0][i] = rgb[3 * i + 0];
        images[1][i] = rgb[3 * i + 1];
        images[2][i] = rgb[3 * i + 2];
    }

    float *image_ptr[3];
    image_ptr[0] = &(images[2].at(0)); // B
    image_ptr[1] = &(images[1].at(0)); // G
    image_ptr[2] = &(images[0].at(0)); // R

    image.images = (unsigned char **)image_ptr;
    image.width = width;
    image.height = height;

    header.num_channels = 3;
    header.channels =
            (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
    // Must be (A)BGR order, since most of EXR viewers expect this channel order.
    strncpy(header.channels[0].name, "B", 255);
    header.channels[0].name[strlen("B")] = '\0';
    strncpy(header.channels[1].name, "G", 255);
    header.channels[1].name[strlen("G")] = '\0';
    strncpy(header.channels[2].name, "R", 255);
    header.channels[2].name[strlen("R")] = '\0';

    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    header.requested_pixel_types =
            (int *)malloc(sizeof(int) * header.num_channels);
    for (int i = 0; i < header.num_channels; i++) {
        header.pixel_types[i] =
                TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
        header.requested_pixel_types[i] =
                TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in
        // .EXR
    }

    const char *err = nullptr;
    int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
        std::cout << err << std::endl;

        FreeEXRErrorMessage(err); // free's buffer for an error message
        return;
    }


    free(header.channels);
    free(header.pixel_types);
    free(header.requested_pixel_types);
}

int main() {
    Dragon::Bitmap bitmap(400, 400);
    for(int i=0;i<400;i++)
        for(int j=0;j<400;j++)
            bitmap.coeffRef(i,j) = Eigen::Vector3f(i/400.f,j/400.f,0.f);

    write_exr(bitmap,"test.exr");
    return 0;
}
