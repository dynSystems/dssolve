# Find OpenGL
find_package(OpenGL REQUIRED)


# Find SDL2
find_package(SDL2 REQUIRED)


# Setup third party libraries which we ship: source and include
set(3RDPARTY_SOURCE_DIR 3rdparty/src)
file(GLOB 3RDPARTY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/include/*)


# Collect source files
# SymbolicC++
file(GLOB SCPP_SOURCE ${CMAKE_SOURCE_DIR}/${3RDPARTY_SOURCE_DIR}/SymbolicCpp/*.cpp)

# nanogui
file(GLOB NANOGUI_SOURCE ${CMAKE_SOURCE_DIR}/${3RDPARTY_SOURCE_DIR}/nanogui/*.cpp)

# nanovg
file(GLOB NANOVG_SOURCE ${CMAKE_SOURCE_DIR}/${3RDPARTY_SOURCE_DIR}/nanovg/*.c)

# dss
set(DSS_SOURCE main.cpp kernel.cpp)


# Assemble the executable
add_executable(dss ${DSS_SOURCE}
                   ${SCPP_SOURCE}
#                    ${NANOGUI_SOURCE}
#                    ${NANOVG_SOURCE}
                   )



# Include and link
target_include_directories(dss PRIVATE ${SDL2_INCLUDE_DIR}
                                       ${OPENGL_INCLUDE_DIR}
                                       ${3RDPARTY_INCLUDE_DIR}
                                       )



target_link_libraries(dss ${OPENGL_gl_LIBRARY}
                          ${SDL2_LIBRARY}
                          )


# Installation
install(TARGETS dss RUNTIME DESTINATION bin)
