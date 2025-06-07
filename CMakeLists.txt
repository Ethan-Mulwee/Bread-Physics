cmake_minimum_required(VERSION 3.10)

project(Bread-Physics)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(IMGUI_SOURCES
  ext/imgui/imgui.cpp
  ext/imgui/imgui_demo.cpp
  ext/imgui/imgui_draw.cpp
  ext/imgui/imgui_tables.cpp
  ext/imgui/imgui_widgets.cpp
  ext/imgui/backends/imgui_impl_glfw.cpp 
  ext/imgui/backends/imgui_impl_opengl3.cpp
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(App demo/main.cpp ${IMGUI_SOURCES} ${SRC_FILES})

add_subdirectory(ext/glfw)
add_subdirectory(ext/glew)

target_include_directories(App 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_SOURCE_DIR}/demo 
  PUBLIC ext/glfw/include
  PUBLIC ext/imgui
  PUBLIC ext/imgui/backends
  PUBLIC ext/Bread-Math/include/bMath
)

target_link_directories(App
  PRIVATE ext/glfw/src
  PRIVATE ext/glew/lib
)

target_link_libraries(App glfw GL GLEW)