cmake_minimum_required(VERSION 3.16)
project(Lili2DExamples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

find_package(Lili2D REQUIRED)

add_executable(hello_sprite_batch
    src/main.cpp
    src/app.cpp
)
target_link_libraries(hello_sprite_batch PRIVATE lili::Lili2D)
