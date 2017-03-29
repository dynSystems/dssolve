enable_testing()
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)


set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)


add_library(serialize SHARED
                             ${PROJECT_SOURCE_DIR}/serialize.cpp)


add_executable(serializationTest
                                 serializationTest.cpp
                                 )

target_include_directories(serializationTest PRIVATE
                                                     ${GTEST_INCLUDE_DIRS}
                                                     ${GMOCK_INCLUDE_DIRS}
                                                     ${PROJECT_SOURCE_DIR}
                                                     )



target_link_libraries(serializationTest
                                        ${GTEST_LIBRARIES}
                                        ${GMOCK_BOTH_LIBRARIES}
                                        serialize)




add_test(NAME AllTests COMMAND
serializationTest
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
)
