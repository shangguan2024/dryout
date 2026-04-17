cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")
endif()

project(dryout)

# Build

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(COPY res/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/res/shaders)
file(COPY res/textures/atlas/ DESTINATION ${CMAKE_BINARY_DIR}/res/textures)

add_subdirectory(src)

# Install

install(DIRECTORY res/shaders/ DESTINATION res/shaders)
install(DIRECTORY res/textures/atlas/ DESTINATION res/textures)

set(CPACK_PACKAGE_NAME "dryout")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_GENERATOR "ZIP")
include(CPack)
