add_library(lili_core OBJECT
    action_map.cpp
    asset_manager.cpp
    clock.cpp
    easing.cpp
    event.cpp
    game.cpp
    game_config.cpp
    keyboard.cpp
    mouse.cpp
    sdl_deleters.cpp
    string_hash.cpp
    thread_pool.cpp
    window.cpp
)
add_library(lili::core ALIAS lili_core)

target_include_directories(lili_core
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(lili_core PUBLIC
    lili_geometry
    SDL3::SDL3
)
