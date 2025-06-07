cmake_minimum_required(VERSION 3.10)

project(Bread-Physics)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(App demo/main.cpp ) #${SRC_FILES}

add_subdirectory(ext/glfw)

target_include_directories(App 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_SOURCE_DIR}/demo 
  PUBLIC ext/glfw/include
)

target_link_directories(App
  PRIVATE ext/glfw/src
)

target_link_libraries(App glfw)