cmake_minimum_required(VERSION 3.24)
project(dragon)

set(CMAKE_CXX_STANDARD 17)
set(embree_DIR D:/Personal/embree/lib/cmake/embree-3.13.5)

include_directories(include)

find_package(Eigen3 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(embree 3 CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)



file(GLOB dragon_src src/dragon/*.cpp src/dragon/core/*.cpp)

add_executable(dragon ${dragon_src})

target_link_libraries(dragon PRIVATE Eigen3::Eigen ZLIB::ZLIB embree TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)

file(GLOB dlls ${EMBREE_ROOT_DIR}/bin/*.dll)
add_custom_command(TARGET dragon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${dlls}
        $<TARGET_FILE_DIR:dragon>)

