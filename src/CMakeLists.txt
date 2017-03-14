# Find OpenGL
find_package(OpenGL REQUIRED)


# Find SDL2
find_package(SDL2 REQUIRED)


# Collect source files
add_executable(dss main.cpp
                   window.cpp)


# Include and link
target_include_directories(dss PRIVATE ${SDL2_INCLUDE_DIR}
                                       ${OPENGL_INCLUDE_DIR})

target_link_libraries(dss ${OPENGL_gl_LIBRARY}
                          ${SDL2_LIBRARY})


# Installation
install(TARGETS dss RUNTIME DESTINATION bin)
