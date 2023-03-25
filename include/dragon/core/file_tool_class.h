#pragma once

#include<filesystem>
#include "common.h"

namespace Dragon
{
    class PathTool{
        typedef std::filesystem::path path;
    private:
        path current_path_;


    public:
        PathTool() {
            current_path_ = std::filesystem::current_path();
        }
        [[nodiscard]] path current_path()const {return current_path_;}
        path GetFilePath(const string& filename){
            return (current_path_/path(filename)).lexically_normal() ;
        }
        void SetCurrentPath(path current_path){current_path_=std::move(current_path);}



    };
}