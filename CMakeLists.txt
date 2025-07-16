cmake_minimum_required(VERSION 3.10)

project(chego)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3.0)

include_directories(${SFML_INCLUDE_DIRS})

add_executable(chego src/main.cpp)

target_link_libraries(chego sfml-graphics sfml-window sfml-system)
target_link_libraries(chego ${CMAKE_SOURCE_DIR}/led_matrix.a)
