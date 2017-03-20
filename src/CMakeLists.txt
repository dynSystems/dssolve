## Collect source files
set(3RDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/src)

# SymbolicC++
file(GLOB SCPP_SOURCE ${3RDPARTY_SOURCE_DIR}/SymbolicCpp/*.cpp)

# nanogui
file(GLOB NANOGUI_SOURCE ${3RDPARTY_SOURCE_DIR}/nanogui/*.cpp)

# nanovg
file(GLOB NANOVG_SOURCE ${3RDPARTY_SOURCE_DIR}/nanovg/*.c)

# glad
file(GLOB GLAD_SOURCE ${3RDPARTY_SOURCE_DIR}/glad/*.c)


# dss
set(DSS_SOURCE
               main.cpp
               kernel.cpp
               gui.cpp
               )


## Assemble the executable
add_executable(dss
                   ${DSS_SOURCE}
                   ${SCPP_SOURCE}
                   ${NANOGUI_SOURCE}
                   ${NANOVG_SOURCE}
                   ${GLAD_SOURCE}
                   )


## Compile options and preprocessor defines
target_compile_options(dss
                           PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++14> $<$<COMPILE_LANGUAGE:C>:-std=c99>
                           PRIVATE -pthread
                           )

target_compile_definitions(dss
                               PRIVATE -DNANOVG_GL2_IMPLEMENTATION
                               PRIVATE -DGL_GLEXT_PROTOTYPES
                               PRIVATE -D_CRT_SECURE_NO_WARNINGS
                               )


## Find includes and link against libraries
set(3RDPARTY_INCLUDE_DIR
                         ${CMAKE_SOURCE_DIR}/3rdparty/include
                         ${CMAKE_SOURCE_DIR}/3rdparty/include/KHR
                         ${CMAKE_SOURCE_DIR}/3rdparty/include/SymbolicCpp
                         ${CMAKE_SOURCE_DIR}/3rdparty/include/nanovg

                         )

# Find threading library with preference to pthreads by compile option -pthread
find_package(Threads REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find SDL2, also finds the threading library
find_package(SDL2 REQUIRED)

# Find Eigen, header only
find_package(Eigen REQUIRED)

# Find ZeroMQ
find_package(ZMQ REQUIRED)

target_include_directories(dss PRIVATE
                                       ${OPENGL_INCLUDE_DIR}
                                       ${SDL2_INCLUDE_DIR}
                                       ${EIGEN_INCLUDE_DIRS}
                                       ${ZMQ_INCLUDE_DIRS}
                                       ${3RDPARTY_INCLUDE_DIR}
                                       )

target_link_libraries(dss
                          Threads::Threads
                          ${OPENGL_gl_LIBRARY}
                          ${SDL2_LIBRARY}
                          ${ZMQ_LIBRARIES}
                          )


## Installation
install(TARGETS dss RUNTIME DESTINATION bin)
