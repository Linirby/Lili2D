add_library(lili_entities OBJECT
    sprite.cpp
)
add_library(lili::entities ALIAS lili_entities)

target_include_directories(lili_entities
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(lili_entities PUBLIC
    lili_core
    lili_ecs
    lili_render
)
