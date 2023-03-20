//
// Created by Dreamoon on 2023/3/20.
//

#ifndef DRAGON_BITMAP_CLASS_H
#define DRAGON_BITMAP_CLASS_H

#pragma once
#include <Eigen/Eigen>
#include "common.h"
#include "vector_class.h"

namespace Dragon{

    class Bitmap:public Eigen::Array<Color3f,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> {
        typedef Eigen::Array<Color3f, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Base;
    private:
        int width_,height_;

    public:
        Bitmap(int width,int height):Base(height,width),width_(width),height_(height){}
        [[nodiscard]] int width() const{return width_;}
        [[nodiscard]] int height() const{return height_;}
        void SetColor(uint x,uint y,Color3f color);
        Color3f GetAverage()const;
        Color3f GetColor(uint x,uint y) const;
    };
}

#endif //DRAGON_BITMAP_CLASS_H
