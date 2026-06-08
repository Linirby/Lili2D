cmake_minimum_required(VERSION 3.16)
project(Lili2DExamples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

find_package(Lili2D REQUIRED)

find_program(GLSLC_COMPILER glslc REQUIRED)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rect.vert.spv
    COMMAND ${GLSLC_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/src/rect.vert -o
        ${CMAKE_CURRENT_BINARY_DIR}/rect.vert.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/rect.vert
)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rect.frag.spv
    COMMAND ${GLSLC_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/src/rect.frag -o
        ${CMAKE_CURRENT_BINARY_DIR}/rect.frag.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/rect.frag
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/text.vert.spv
    COMMAND ${GLSLC_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/src/text.vert -o
        ${CMAKE_CURRENT_BINARY_DIR}/text.vert.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/text.vert
)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/text.frag.spv
    COMMAND ${GLSLC_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/src/text.frag -o
        ${CMAKE_CURRENT_BINARY_DIR}/text.frag.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/text.frag
)

add_custom_target(compile_shaders ALL DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/rect.vert.spv
    ${CMAKE_CURRENT_BINARY_DIR}/rect.frag.spv
    ${CMAKE_CURRENT_BINARY_DIR}/text.vert.spv
    ${CMAKE_CURRENT_BINARY_DIR}/text.frag.spv
)

add_executable(hello_shader
    src/main.cpp
    src/app.cpp
)
add_dependencies(hello_shader compile_shaders)

target_link_libraries(hello_shader PRIVATE lili::Lili2D)
