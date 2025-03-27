cmake_minimum_required(VERSION 3.30)
project(xuan)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(gsl-lite CONFIG REQUIRED)
find_package(libassert CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

if (USE_INSTALLED_DAWN)
    find_package(dawn CONFIG REQUIRED)
    add_library(webgpu ALIAS dawn::webgpu_dawn) # Expected by sdl3webgpu
else()
    add_subdirectory(webgpu)
endif()

add_subdirectory(sdl3webgpu)
add_subdirectory(source)