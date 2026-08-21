add_executable(lili2d_tests
    geometry_tests.cpp
    ecs_tests.cpp
    physics_tests.cpp
    core_tests.cpp
)

target_link_libraries(lili2d_tests PRIVATE
    Lili2D
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(lili2d_tests)
