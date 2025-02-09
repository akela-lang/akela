set(PROJECT_VERSION 0.0.0)

## akela
install(TARGETS akela
        EXPORT akela-targets
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

export(EXPORT akela-targets
        FILE "${CMAKE_BINARY_DIR}/akela-akela-targets.cmake"
        NAMESPACE akela::
)

## cobble
install(TARGETS cobble
        EXPORT cobble-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(EXPORT cobble-targets
        FILE akela-cobble-targets.cmake
        NAMESPACE akela::
        DESTINATION lib/cmake/akela
)

export(EXPORT cobble-targets
        FILE "${CMAKE_BINARY_DIR}/akela-cobble-targets.cmake"
        NAMESPACE akela::
)

## json
install(TARGETS json
        EXPORT json-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(EXPORT json-targets
        FILE akela-json-targets.cmake
        NAMESPACE akela::
        DESTINATION lib/cmake/akela
)

export(EXPORT json-targets
        FILE "${CMAKE_BINARY_DIR}/akela-json-targets.cmake"
        NAMESPACE akela::
)

## zinc
install(TARGETS zinc
        EXPORT zinc-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

export(EXPORT zinc-targets
        FILE "${CMAKE_BINARY_DIR}/akela-zinc-targets.cmake"
        NAMESPACE akela::
)

install(EXPORT zinc-targets
        FILE akela-zinc-targets.cmake
        NAMESPACE akela::
        DESTINATION lib/cmake/akela
)

## config
include(CMakePackageConfigHelpers)
configure_package_config_file(
        cmake/akela-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/akela-config.cmake
        INSTALL_DESTINATION lib/cmake/akela
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/akela-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)
