//
// Created by Dreamoon on 2023/3/20.
//
#include "dragon/core/bitmap_class.h"

#include <utility>

namespace Dragon {

    void Bitmap::SetColor(uint x, uint y, Color3f color) {
        this->coeffRef(height()-y, x) = color;
    }

    Color3f Bitmap::GetColor(uint x, uint y) const {
        return this->coeff(x, y);
    }

    Color3f Bitmap::GetAverage() const {
        Color3f v = mean();
        return {v.x(), v.y(), v.z()};
    }
}