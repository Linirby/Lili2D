add_library(lili_physics OBJECT
    aabb_collider.cpp
    circle_collider.cpp
)
add_library(lili::physics ALIAS lili_physics)

target_include_directories(lili_physics
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(lili_physics PUBLIC lili_geometry)
