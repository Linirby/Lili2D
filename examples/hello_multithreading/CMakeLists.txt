cmake_minimum_required(VERSION 3.16)
project(hello_multithreading)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

find_package(Lili2D REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../../build" NO_DEFAULT_PATH)

add_executable(hello_multithreading
    src/main.cpp
    src/game.cpp
    src/systems.cpp
)
target_link_libraries(hello_multithreading PRIVATE lili::Lili2D)
