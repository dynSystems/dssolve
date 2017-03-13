cmake_minimum_required(VERSION 2.6)
project(dss)

add_executable(dss main.cpp)

install(TARGETS dss RUNTIME DESTINATION bin)
