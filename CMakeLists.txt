cmake_minimum_required(VERSION 3.23)
project(tetris_cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES main.cpp game/*.cpp engine/*.cpp)
find_package(PkgConfig)
find_package(SDL2)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE include ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})



