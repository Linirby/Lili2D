add_library(lili_render OBJECT
    2d/animated_sprite.cpp
    2d/circle.cpp
    2d/line.cpp
    2d/rect.cpp
    2d/sprite.cpp
    2d/sprite_batch.cpp
    2d/text.cpp
    2d/ui_layout.cpp
    common/animation.cpp
    common/animation_player.cpp
    common/animation_registry.cpp
    common/atlas_map.cpp
    common/camera.cpp
    common/material.cpp
    common/material_registry.cpp
    common/renderable.cpp
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
