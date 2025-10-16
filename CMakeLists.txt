cmake_minimum_required(VERSION 3.10)

project(Bread-Physics)

file(GLOB_RECURSE ENGINE_SRC_FILES src/*.cpp)
file(GLOB_RECURSE DEMO_SRC_FILES demo/*.cpp)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(${CMAKE_BUILD_TYPE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_definitions(BPHYSICS_DEBUG)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

add_subdirectory(ext/brl)

# in the future build the physics engine to a library and include/link it

add_executable(Bread-Physics-Demo ${DEMO_SRC_FILES} ${ENGINE_SRC_FILES})

# target_compile_options(App PRIVATE -Wall)

target_include_directories(Bread-Physics-Demo 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include 
  ${CMAKE_CURRENT_SOURCE_DIR}/demo 
  PUBLIC ext/brl/include
  PUBLIC ext/Simple-Math/smath
)

target_link_directories(Bread-Physics-Demo
  PRIVATE ext/brl
)

target_link_libraries(Bread-Physics-Demo brl)