cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pthread")

project(shadow_baking)

add_compile_options(-pthread -Wall -Wextra -Wpedantic -Wundef -O3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")

add_executable(shadow_baking main.cpp)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS} 
    ${PROJECT_SOURCE_DIR}/glm
)
target_link_libraries(shadow_baking ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})