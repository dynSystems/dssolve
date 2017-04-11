# Exceptions
add_library(dsexct INTERFACE)
target_include_directories(dsexct
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

install(TARGETS dsexct EXPORT dsexctConfig)
# FIXME: Once we ship more complex libs, we must use wildcards or subfolders.
install(FILES include/exceptions.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT dsexctConfig DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/)
export(TARGETS dsexct FILE dsexctConfig.cmake)


# Serialization library
add_library(dsser SHARED src/serialize.cpp)
target_include_directories(dsser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

target_link_libraries(dsser PUBLIC dsexct)
# FIXME:Move this to target_compile_features once cmake 3.8 has landed.
target_compile_options(dsser PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++14> $<$<COMPILE_LANGUAGE:C>:-std=c11>)

install(TARGETS dsser EXPORT dsserConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
# FIXME: Once we ship more complex libs, we must use wildcards or subfolders.
install(FILES include/serialize.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT dsserConfig DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/)
export(TARGETS dsser FILE dsserConfig.cmake)


# Add tests
add_subdirectory(tests/)
