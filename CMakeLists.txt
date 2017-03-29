cmake_minimum_required(VERSION 3.7)
project(dss)


# Set path for own cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules/)


# Add our sources
add_subdirectory(src/)


# Add tests
add_subdirectory(tests/)
