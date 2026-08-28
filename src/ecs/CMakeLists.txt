add_library(lili_ecs OBJECT
    command_buffer.cpp
    ecs_registry.cpp
)
add_library(lili::ecs ALIAS lili_ecs)

target_include_directories(lili_ecs
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)
