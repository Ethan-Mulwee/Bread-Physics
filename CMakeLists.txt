cmake_minimum_required(VERSION 3.10)

project(Bread-Physics)

file(GLOB_RECURSE ENGINE_SRC_FILES src/*.cpp)
file(GLOB_RECURSE DEMO_SRC_FILES demo/*.cpp)


set(IMGUI_SOURCES
  ext/imgui/imgui.cpp
  ext/imgui/imgui_demo.cpp
  ext/imgui/imgui_draw.cpp
  ext/imgui/imgui_tables.cpp
  ext/imgui/imgui_widgets.cpp
  ext/imgui/backends/imgui_impl_glfw.cpp 
  ext/imgui/backends/imgui_impl_opengl3.cpp
)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(${CMAKE_BUILD_TYPE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# in the future build the physics engine to a library and include/link it

set( glew-cmake_BUILD_SHARED FALSE )
set( ONLY_LIBS TRUE )

add_subdirectory(ext/glew)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory(ext/glfw)


add_executable(Bread-Physics-Demo ${DEMO_SRC_FILES} ${IMGUI_SOURCES} ${ENGINE_SRC_FILES})

# target_compile_options(App PRIVATE -Wall)

target_include_directories(Bread-Physics-Demo 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_SOURCE_DIR}/demo 
  PUBLIC ext/glfw/include
  PUBLIC ext/glew/include
  PUBLIC ext/imgui
  PUBLIC ext/imgui/backends
  PUBLIC ext/Bread-Math/include/bMath
  PUBLIC ext/Simple-Math/smath
)

target_link_directories(Bread-Physics-Demo
  PRIVATE ext/glfw/src
  PRIVATE ext/glew/lib
)

target_link_libraries(Bread-Physics-Demo glfw libglew_static)