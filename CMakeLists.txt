cmake_minimum_required(VERSION 3.10)

project(Bread-Math)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Cubes demo/raylib/cubes.cpp demo/raylib/render.cpp demo/raylib/render.hpp ${SRC_FILES})
target_include_directories(Cubes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/demo/raylib)
target_link_libraries(Cubes PUBLIC /usr/local/lib/libraylib.so)

add_executable(Drag demo/raylib/dragcubes.cpp demo/raylib/render.cpp demo/raylib/render.hpp ${SRC_FILES})
target_include_directories(Drag PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/demo/raylib)
target_link_libraries(Drag PUBLIC /usr/local/lib/libraylib.so)