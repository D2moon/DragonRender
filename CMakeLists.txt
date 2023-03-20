cmake_minimum_required(VERSION 3.24)
project(dragon)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(Eigen3 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)


file(GLOB dragon_src src/dragon/*.cpp src/dragon/core/*.cpp)

add_executable(dragon ${dragon_src})

target_link_libraries(dragon PRIVATE Eigen3::Eigen ZLIB::ZLIB)

