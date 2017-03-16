# Find OpenGL
find_package(OpenGL REQUIRED)


# Find SDL2
find_package(SDL2 REQUIRED)


# # Find GiNaC
# find_path(GINAC_INCLUDE_DIR symbolicc++.h)
# find_library(GINAC_LIBRARY symbolicc++ HINTS /home/wmader/local/lib)
# message(STATUS ${SYMPATH})


# Set variables for 3rd party includes
set(SYMBOLICCPP_INCLUDE_DIR 3rdparty/SymbolicCpp/include)
set(SYMBOLICCPP_SOURCE_DIR 3rdparty/SymbolicCpp/src)


# Collect source files
# SymbolicCpp
file(GLOB SCPP_SOURCE ${CMAKE_SOURCE_DIR}/${SYMBOLICCPP_SOURCE_DIR}/*.cpp)
add_executable(dss ${SCPP_SOURCE}
                   main.cpp
                   window.cpp)



# Include and link
target_include_directories(dss PRIVATE ${SDL2_INCLUDE_DIR}
                                       ${OPENGL_INCLUDE_DIR}
                                       ${CMAKE_SOURCE_DIR}/${SYMBOLICCPP_INCLUDE_DIR}
                                       )



target_link_libraries(dss ${OPENGL_gl_LIBRARY}
                          ${SDL2_LIBRARY}
                          )


# Installation
install(TARGETS dss RUNTIME DESTINATION bin)
