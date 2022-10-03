cmake_minimum_required(VERSION 3.23)
project(tetris_cpp)

set(CMAKE_CXX_STANDARD 17)

# Include FindSDL2 and FindSDL2TTF
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SRC_FILES main.cpp game/*.cpp engine/*.cpp ai/*.cpp)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})



