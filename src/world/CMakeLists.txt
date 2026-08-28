add_library(lili_world OBJECT
    camera.cpp
    chunk.cpp
    tile_registry.cpp
    tilemap.cpp
)
add_library(lili::world ALIAS lili_world)

target_include_directories(lili_world
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(lili_world PUBLIC
    lili_render
    SDL3::SDL3
)
