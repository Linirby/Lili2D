add_library(lili_render OBJECT
    core/gpu_mesh.cpp
    core/shader.cpp
    core/texture.cpp
    interfaces/renderable.cpp
    passes/main_render_pass.cpp
    pipelines/main_graphics_pipeline.cpp
    scene/common/animation.cpp
    scene/common/animation_player.cpp
    scene/common/animation_registry.cpp
    scene/common/atlas_map.cpp
    scene/common/material.cpp
    scene/common/material_registry.cpp
    scene/common/model.cpp
    scene/common/text.cpp
    scene/common/utils.cpp
    scene/core2d/animated_sprite.cpp
    scene/core2d/sprite.cpp
    scene/core2d/sprite_batch.cpp
    scene/shapes/circle.cpp
    scene/shapes/line.cpp
    scene/shapes/rect.cpp
    ui/ui_layout.cpp
    renderer.cpp
    ${EMBEDDED_ASSETS}
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
    lili_geometry
    lili_core
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_shadercross::SDL3_shadercross
)
