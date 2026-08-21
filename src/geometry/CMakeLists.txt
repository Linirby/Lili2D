add_library(lili_geometry OBJECT
    mat3x3.cpp
    mat4x4.cpp
    point2.cpp
    point3.cpp
    utils.cpp
    vec2.cpp
    vec3.cpp
    vec4.cpp
)
add_library(lili::geometry ALIAS lili_geometry)

target_include_directories(lili_geometry
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)
