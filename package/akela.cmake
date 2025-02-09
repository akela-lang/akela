target_include_directories(akela PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS akela zinc
        EXPORT akela-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(TARGETS zinc
        EXPORT zinc-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(EXPORT akela-targets
        FILE akela-akela-targets.cmake
        NAMESPACE akela::
        DESTINATION lib/cmake/akela
)

install(EXPORT zinc-targets
        FILE akela-zinc-targets.cmake
        NAMESPACE akela::
        DESTINATION lib/cmake/akela
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
        cmake/akela-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/akela-config.cmake
        INSTALL_DESTINATION lib/cmake/akela
)

export(EXPORT akela-targets
        FILE "${CMAKE_BINARY_DIR}/akela-akela-targets.cmake"
        NAMESPACE akela::
)

export(EXPORT zinc-targets
        FILE "${CMAKE_BINARY_DIR}/akela-zinc-targets.cmake"
        NAMESPACE akela::
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/akela-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)
