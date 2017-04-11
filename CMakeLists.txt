cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(dss VERSION 0.0.1 LANGUAGES C CXX)

## Project-wide settings
# Path to own cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules/)

# Enable google testing framework
enable_testing()

# Load platform-independent installation paths
include(GNUInstallDirs)

# Add our libraries
add_subdirectory(lib/)

# Add our sources
add_subdirectory(src/)
