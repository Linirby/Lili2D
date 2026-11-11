add_library(lili_render OBJECT
    2d/batch_helper.cpp
    2d/bitmap_font.cpp
    2d/render2d_system.cpp
    2d/dynamic_batch.cpp
    2d/static_batch.cpp
    2d/ui_layout_system.cpp
    common/animation.cpp
    common/animation_registry.cpp
    common/animation_system.cpp
    common/atlas_map.cpp
    common/camera.cpp
    common/material.cpp
    common/material_registry.cpp
    common/unit_mesh.cpp
    gpu/gpu_mesh.cpp
    gpu/main_graphics_pipeline.cpp
    gpu/main_render_pass.cpp
    gpu/shader.cpp
    gpu/texture.cpp
    renderer.cpp
)
add_library(lili::render ALIAS lili_render)

target_include_directories(lili_render
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(lili_render PUBLIC
    lili_core
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_shadercross::SDL3_shadercross
)
