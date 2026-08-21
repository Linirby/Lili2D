cmake_minimum_required(VERSION 3.14)
project(Lili2D VERSION 0.1.2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_shadercross REQUIRED CONFIG)

# Embedded assets (e.g. 1x1 white fallback pixel)
set(ASSET_FILES
    assets/white_1x1.png
)
set(EMBEDDED_ASSETS "")
foreach(ASSET IN LISTS ASSET_FILES)
    get_filename_component(ASSET_NAME ${ASSET} NAME)
    string(REPLACE "." "_" VAR_NAME "${ASSET_NAME}")
    set(
        HEADER_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/lili2d/render/${VAR_NAME}.hpp"
    )
    add_custom_command(
        OUTPUT ${HEADER_FILE}
        COMMAND ${CMAKE_COMMAND}
            -DINPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}"
            -DOUTPUT_FILE="${HEADER_FILE}" -DVAR_NAME="${VAR_NAME}" -P
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake"
        COMMENT "Embedding asset ${ASSET_NAME}"
    )
    list(APPEND EMBEDDED_ASSETS ${HEADER_FILE})
endforeach()

# Engine Submodules
add_subdirectory(src)

# Unit Tests
option(LILI2D_BUILD_TESTS "Build Lili2D unit tests" ON)

if(LILI2D_BUILD_TESTS)
    enable_testing()
    find_package(Catch2 3 QUIET)
    if(NOT Catch2_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.5.2
        )
        FetchContent_MakeAvailable(Catch2)
    endif()
    add_subdirectory(tests)
endif()

# Installation & CMake Package Configuration
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS Lili2D
    EXPORT Lili2DTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
export(TARGETS Lili2D
    NAMESPACE lili::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/Lili2DTargets.cmake"
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT Lili2DTargets
    FILE Lili2DTargets.cmake
    NAMESPACE lili::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lili2D
)

configure_package_config_file(
    cmake/Lili2DConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lili2D
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Lili2DConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lili2D
)
