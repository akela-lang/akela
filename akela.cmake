set(PROJECT_VERSION 0.0.0)

set(AKELA_AVAILABLE_COMPONENTS zinc lava json cobble centipede coverage dataframe akela akela-llvm)

foreach(comp IN LISTS AKELA_AVAILABLE_COMPONENTS)
    install(TARGETS ${comp}
            EXPORT ${comp}-targets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            INCLUDES DESTINATION include
    )

    install(EXPORT ${comp}-targets
            FILE akela-${comp}-targets.cmake
            NAMESPACE akela::
            DESTINATION lib/cmake/akela
    )

    export(EXPORT ${comp}-targets
            FILE "${CMAKE_BINARY_DIR}/akela-${comp}-targets.cmake"
            NAMESPACE akela::
    )
endforeach ()

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
