cmake_minimum_required(VERSION 3.16)
project(Control)

set(CMAKE_CXX_STANDARD 17)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
# SDL3
add_subdirectory(external/SDL)
# Dear ImGui
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
    ${CMAKE_SOURCE_DIR}/external/SDL/include
)
target_link_libraries(imgui PUBLIC SDL3::SDL3)
# OpenCV
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/external/opencv/build")
find_package(OpenCV REQUIRED)

add_executable(control
    main.cpp
    application.cpp
    editorapplication.cpp
)
target_include_directories(control PRIVATE
    ${CMAKE_SOURCE_DIR}/external/opencv/include
)
target_link_libraries(control
    PRIVATE
        imgui
        ${OpenCV_LIBS}
        SDL3::SDL3
)
