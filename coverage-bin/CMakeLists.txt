set(coverage_bin_files
        coverage.c
)

add_executable(coverage-bin ${coverage_bin_files})
target_link_libraries(coverage-bin coverage cobble zinc)
set_target_properties(coverage-bin
        PROPERTIES
        OUTPUT_NAME coverage
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)