add_library(lili_scene OBJECT
    scene.cpp
    scene_manager.cpp
)
add_library(lili::scene ALIAS lili_scene)

target_include_directories(lili_scene
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(lili_scene PUBLIC
    lili_core
    lili_render
    SDL3::SDL3
)
