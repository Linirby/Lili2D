cmake_minimum_required(VERSION 3.16)
project(lili2D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

# ==============================================================================
#  > TIER 1
# ==============================================================================

# Lili Core
add_library(LiliCore STATIC
	src/core/clock.cpp
	src/core/sdl_system.cpp
	src/core/window.cpp
	src/core/event.cpp
	src/core/keyboard.cpp
	src/core/mouse.cpp
)
target_include_directories(LiliCore PUBLIC src)
target_link_libraries(LiliCore PRIVATE SDL3::SDL3)

# Lili Geometry
add_library(LiliGeometry STATIC
	src/geometry/mat4x4.cpp
	src/geometry/vec2.cpp
	src/geometry/vec3.cpp
	src/geometry/vec4.cpp
)
target_include_directories(LiliGeometry PUBLIC src)

# ==============================================================================
#  > TIER 2
# ==============================================================================

# Lili Physics
add_library(LiliPhysics STATIC
	src/physics/collision.cpp
)
target_include_directories(LiliPhysics PUBLIC src)
target_link_libraries(LiliPhysics PRIVATE LiliGeometry)

# ==============================================================================
#  > TIER 3
# ==============================================================================

# Lili Render
# add_library(LiliRender STATIC
# 	src/render/renderer.cpp
#
# 	src/render/core/gpu_mesh.cpp
# 	src/render/core/shader.cpp
# 	src/render/core/texture.cpp
#
# 	src/render/passes/world_pass.cpp
# 	src/render/passes/ui_pass.cpp
#
# 	src/render/pipelines/ui_pipeline.cpp
# 	src/render/pipelines/world_pipeline.cpp
#
# 	src/render/scene/camera.cpp
# 	src/render/scene/model.cpp
# 	src/render/scene/sprite.cpp
# 	src/render/scene/ui_text.cpp
# )
# target_include_directories(LiliRender PUBLIC src)
# target_link_libraries(LiliRender PRIVATE
# 	LiliGeometry
# 	SDL3::SDL3
# 	SDL3_image::SDL3_image
# )

# ==============================================================================
#  > TIER 5
# ==============================================================================

# Example App
add_executable(hello_sprite
	examples/hello_sprite/main.cpp
	examples/hello_sprite/app.cpp
)
target_include_directories(hello_sprite PRIVATE src)
target_link_libraries(hello_sprite PRIVATE
	LiliCore
	LiliPhysics
	SDL3::SDL3
)
