cmake_minimum_required(VERSION 3.16)
project(Control)

set(CMAKE_CXX_STANDARD 17)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

add_subdirectory(external/SDL)
add_library(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
add_executable(control main.cpp)

target_link_libraries(control PRIVATE SDL3::SDL3 imgui)
