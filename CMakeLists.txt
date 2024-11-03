cmake_minimum_required(VERSION 3.10)

project(Bread-Math)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_library(bMath ${SRC_FILES})
target_include_directories(bMath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Main Main.cpp)
target_link_libraries(Main PUBLIC bMath)
target_link_libraries(Main PUBLIC /usr/local/lib/libraylib.so)