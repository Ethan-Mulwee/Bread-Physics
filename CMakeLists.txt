cmake_minimum_required(VERSION 3.10)

project(Bread-Math)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(CMAKE_BUILD_TYPE Debug)

add_executable(Main Main.cpp ${SRC_FILES})
target_include_directories(Main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Main PUBLIC /usr/local/lib/libraylib.so)