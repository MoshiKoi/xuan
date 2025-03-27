add_library(WebGPU)
target_sources(WebGPU PUBLIC FILE_SET CXX_MODULES FILES webgpu.cpp)
target_link_libraries(WebGPU PRIVATE 
    libassert::assert
    webgpu)

add_library(App)
target_sources(App PUBLIC FILE_SET CXX_MODULES FILES app.cpp)
target_link_libraries(App PRIVATE
    WebGPU
    webgpu
    SDL3::SDL3
    sdl3webgpu
    magic_enum::magic_enum
    libassert::assert
    spdlog::spdlog)

add_executable(xuan main.cpp)
target_link_libraries(xuan PRIVATE
    App
    SDL3::SDL3
    webgpu
    gsl::gsl-lite-v1
    spdlog::spdlog)

add_custom_command(TARGET xuan POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:xuan> $<TARGET_RUNTIME_DLLS:xuan>
    COMMAND_EXPAND_LISTS)