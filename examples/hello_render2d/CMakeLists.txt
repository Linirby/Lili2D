cmake_minimum_required(VERSION 3.10)
project(Lili2DExamples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -O3)

find_package(Lili2D REQUIRED)

add_executable(hello_render2d
    src/main.cpp
    src/app.cpp
    src/rng_helper.cpp
)
target_link_libraries(hello_render2d PRIVATE lili::Lili2D)

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/EmbarkAssets.cmake")
embark_target_assets(hello_render2d)
