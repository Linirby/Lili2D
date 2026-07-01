cmake_minimum_required(VERSION 3.10)
project(Lili2D VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

add_library(Lili2D SHARED)

find_program(GLSLC_COMPILER glslc REQUIRED)
set(SHADER_FILES
    src/shader/world_2d.vert
    src/shader/world_2d.frag
)
set(EMBEDDED_SHADERS "")
foreach(SHADER IN LISTS SHADER_FILES)
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    string(REPLACE "." "_" VAR_NAME "${SHADER_NAME}_spv")
    set(SPV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/${SHADER_NAME}.spv")
    set(HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/${VAR_NAME}.hpp")
    add_custom_command(
        OUTPUT ${HEADER_FILE}
        COMMAND ${GLSLC_COMPILER} "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}" -o
            "${SPV_FILE}"
        COMMAND ${CMAKE_COMMAND} -DINPUT_FILE="${SPV_FILE}"
            -DOUTPUT_FILE="${HEADER_FILE}" -DVAR_NAME="${VAR_NAME}" -P
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake"
        COMMENT "Compiling and embedding shader ${SHADER_NAME}"
    )
    list(APPEND EMBEDDED_SHADERS ${HEADER_FILE})
endforeach()

set(ASSET_FILES
    assets/white_1x1.png
)
set(EMBEDDED_ASSETS "")
foreach(ASSET IN LISTS ASSET_FILES)
    get_filename_component(ASSET_NAME ${ASSET} NAME)
    string(REPLACE "." "_" VAR_NAME "${ASSET_NAME}")
    set(HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/render/${VAR_NAME}.hpp")
    add_custom_command(
        OUTPUT ${HEADER_FILE}
        COMMAND ${CMAKE_COMMAND}
            -DINPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}"
            -DOUTPUT_FILE="${HEADER_FILE}" -DVAR_NAME="${VAR_NAME}" -P
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake"
        COMMENT "Embedding asset ${ASSET_NAME}"
    )
    list(APPEND EMBEDDED_ASSETS ${HEADER_FILE})
endforeach()

target_sources(Lili2D PRIVATE
    ${EMBEDDED_SHADERS}
    ${EMBEDDED_ASSETS}

    src/geometry/mat3x3.cpp
    src/geometry/mat4x4.cpp
    src/geometry/vec2.cpp
    src/geometry/vec3.cpp
    src/geometry/vec4.cpp
    src/geometry/point2.cpp
    src/geometry/point3.cpp
    src/geometry/utils.cpp

    src/core/clock.cpp
    src/core/ecs.cpp
    src/core/window.cpp
    src/core/event.cpp
    src/core/keyboard.cpp
    src/core/mouse.cpp

    src/render/core/gpu_mesh.cpp
    src/render/core/shader.cpp
    src/render/core/texture.cpp

    src/render/passes/ui_pass.cpp
    src/render/passes/world_2d_pass.cpp

    src/render/pipelines/ui_pipeline.cpp
    src/render/pipelines/world_2d_pipeline.cpp

    src/render/scene/common/animation.cpp
    src/render/scene/common/atlas_map.cpp
    src/render/scene/common/material.cpp
    src/render/scene/common/model.cpp
    src/render/scene/common/text.cpp
    src/render/scene/common/utils.cpp

    src/render/scene/core2d/animated_sprite.cpp
    src/render/scene/core2d/sprite.cpp
    src/render/scene/core2d/sprite_batch.cpp

    src/render/scene/shapes/circle.cpp
    src/render/scene/shapes/line.cpp
    src/render/scene/shapes/rect.cpp

    src/render/renderer.cpp

    src/physics/collision.cpp

    src/scene/scene.cpp
    src/scene/scene_manager.cpp

    src/world/camera.cpp
    src/world/chunk.cpp
    src/world/tile.cpp
    src/world/tile_registry.cpp
    src/world/tilemap.cpp
)

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    set_target_properties(Lili2D PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "IPO is not supported: ${output}")
endif()

target_include_directories(Lili2D
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(Lili2D PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
install(TARGETS Lili2D
    EXPORT Lili2DTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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
