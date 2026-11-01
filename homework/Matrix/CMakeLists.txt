CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(Matrix)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)


add_executable(Matrix main.cpp src/Matrix.cpp)
