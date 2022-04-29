cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pthread")

project(voxel_terrain)

add_compile_options(-pthread -Wall -Wextra -Wpedantic -Wundef -O2)

file(GLOB helloworld_SRC
     "*.h"
     "*.cpp"
)

add_executable(voxel_terrain ${helloworld_SRC})
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(voxel_terrain ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})