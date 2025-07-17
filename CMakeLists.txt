cmake_minimum_required(VERSION 3.10)
project(chego)
set(CMAKE_CXX_STANDARD 17)


find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})

# Создаем библиотеку (если есть .cpp-файлы, добавьте их)

# add_library(board_headers INTERFACE)
# target_include_directories(board_headers INTERFACE
#     ${CMAKE_CURRENT_SOURCE_DIR}/board
# )

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system)
# target_link_libraries(${PROJECT_NAME} PUBLIC matrix_headers)
# target_link_libraries(${PROJECT_NAME} PUBLIC board_headers)