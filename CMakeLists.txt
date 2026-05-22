cmake_minimum_required(VERSION 3.16)
project(Lili2D VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Wextra)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

# ==============================================================================
#  > TIER 1
# ==============================================================================

add_library(LiliCore OBJECT
    src/core/clock.cpp
    src/core/sdl_system.cpp
    src/core/window.cpp
    src/core/event.cpp
    src/core/keyboard.cpp
    src/core/mouse.cpp
)
target_include_directories(LiliCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/lili2d>
)
target_link_libraries(LiliCore PRIVATE SDL3::SDL3)

add_library(LiliGeometry OBJECT
    src/geometry/mat3x3.cpp
    src/geometry/mat4x4.cpp
    src/geometry/vec2.cpp
    src/geometry/vec3.cpp
    src/geometry/vec4.cpp
)
target_include_directories(LiliGeometry PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/lili2d>
)

# ==============================================================================
#  > TIER 2
# ==============================================================================

add_library(LiliPhysics OBJECT
    src/physics/collision.cpp
)
target_include_directories(LiliPhysics PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/lili2d>
)
target_link_libraries(LiliPhysics PRIVATE LiliGeometry)

# ==============================================================================
#  > TIER 3
# ==============================================================================

add_library(LiliRender OBJECT
    src/render/core/gpu_mesh.cpp
    src/render/core/shader.cpp
    src/render/core/texture.cpp

    src/render/passes/ui_pass.cpp

    src/render/pipelines/ui_pipeline.cpp

    src/render/scene/camera.cpp
    src/render/scene/material.cpp
    src/render/scene/model.cpp
    src/render/scene/sprite.cpp
    src/render/scene/ui_text.cpp

    src/render/renderer.cpp
)
target_include_directories(LiliRender PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/lili2d>
)
target_link_libraries(LiliRender PRIVATE
    LiliGeometry
    SDL3::SDL3
    SDL3_image::SDL3_image
)

# ==============================================================================
#  > SHARED LIBRARY
# ==============================================================================

add_library(Lili2D SHARED)
target_sources(Lili2D PRIVATE
    $<TARGET_OBJECTS:LiliCore>
    $<TARGET_OBJECTS:LiliGeometry>
    $<TARGET_OBJECTS:LiliPhysics>
    $<TARGET_OBJECTS:LiliRender>
)
target_link_libraries(Lili2D
    PRIVATE
        LiliCore
        LiliGeometry
        LiliPhysics
        LiliRender
    PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
)
target_include_directories(Lili2D PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/lili2d>
)

# ==============================================================================
#  > INSTALL
# ==============================================================================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS Lili2D
    EXPORT Lili2DTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lili2d
    FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT Lili2DTargets
    FILE Lili2DTargets.cmake
    NAMESPACE lili::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lili2D
)

configure_package_config_file(
    cmake/Lili2DConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lili2D
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lili2D
)
