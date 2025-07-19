cmake_minimum_required(VERSION 3.16)
project(Control)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/SDL)

add_executable(control main.cpp)

target_link_libraries(control PRIVATE SDL3::SDL3)
