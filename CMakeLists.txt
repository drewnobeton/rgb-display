cmake_minimum_required(VERSION 3.16)

project(rgb-display)

add_executable(rgb-display src/rgb-display.cpp)

add_subdirectory(lib/SDL3-3.2.28)

target_include_directories(rgb-display 
    PUBLIC 
        lib/SDL3-3.2.28/include
    )
target_link_libraries(rgb-display SDL3::SDL3)