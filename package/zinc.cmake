target_include_directories(zinc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/zinc>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(zinc PROPERTIES EXPORT_NAME zinc)

install(TARGETS zinc
        EXPORT zincTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

export(EXPORT zincTargets
        FILE "${CMAKE_BINARY_DIR}/ZincTargets.cmake"
        NAMESPACE zinc::
)

install(EXPORT zincTargets
        FILE ZincTargets.cmake
        NAMESPACE zinc::
        DESTINATION lib/cmake/akela
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
        cmake/ZincConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/ZincConfig.cmake
        INSTALL_DESTINATION lib/cmake/zinc
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/ZincConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ZincConfigVersion.cmake
        DESTINATION lib/cmake/Zinc
)