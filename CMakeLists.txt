cmake_minimum_required(VERSION 3.10)
project(chego)
set(CMAKE_CXX_STANDARD 17)


find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)
find_package(cpr REQUIRED)
find_package(OpenSSL REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources)

# Копируем шрифт в resources
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/arial.ttf
    ${CMAKE_CURRENT_BINARY_DIR}/resources/arial.ttf
    COPYONLY
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)