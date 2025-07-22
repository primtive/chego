cmake_minimum_required(VERSION 3.10)
project(chego)
set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)
find_package(SDL2 REQUIRED)
find_package(cpr REQUIRED)
find_package(OpenSSL REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

set(PIO_ENV_NAME env1)
set(PIO_LIB_DIR "${CMAKE_SOURCE_DIR}/.pio/libdeps/${PIO_ENV_NAME}")
include_directories(
    ${PIO_LIB_DIR}/U8g2/src
    ${PIO_LIB_DIR}/U8g2/src/clib
)
file(GLOB U8G2_SOURCES 
    ${PIO_LIB_DIR}/U8g2/src/*.cpp
    ${PIO_LIB_DIR}/U8g2/src/clib/*.c
)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources)
# Копируем шрифт в resources
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/arial.ttf
    ${CMAKE_CURRENT_BINARY_DIR}/resources/arial.ttf
    COPYONLY
)

add_executable(${PROJECT_NAME} src/main.cpp ${U8G2_SOURCES})

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     ENVIRONMENT "SDL_RENDER_DRIVER=opengles2;SDL_VIDEODRIVER=x11;SDL_VIDEODRIVER=wayland"
# )

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})