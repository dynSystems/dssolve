find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

add_executable(serializationTest serializationTest.cpp)
target_include_directories(serializationTest PRIVATE
                                                     ${GTEST_INCLUDE_DIRS}
                                                     ${GMOCK_INCLUDE_DIRS}
                                                     ${DSSER_INCLUDE_DIRS}
                                                     )

target_link_libraries(serializationTest
                                        ${GTEST_LIBRARIES}
                                        ${GMOCK_BOTH_LIBRARIES}
                                        dsexct
                                        dsser
                                        )

add_test(NAME LibraryTests COMMAND serializationTest WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
