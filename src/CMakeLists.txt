add_subdirectory(ecs)
add_subdirectory(core)
add_subdirectory(render)
add_subdirectory(world)
add_subdirectory(scene)

add_library(Lili2D SHARED)
add_library(lili::Lili2D ALIAS Lili2D)
add_library(lili::lili2d ALIAS Lili2D)

target_link_libraries(Lili2D
    PRIVATE
        lili_ecs
        lili_core
        lili_render
        lili_world
        lili_scene
    PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_shadercross::SDL3_shadercross
)

target_include_directories(Lili2D
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    set_target_properties(Lili2D PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "IPO is not supported: ${output}")
endif()
