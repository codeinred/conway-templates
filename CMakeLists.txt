cmake_minimum_required(VERSION 3.14)

project(conway_templates LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(conway src/main.cpp src/conway.cpp)
