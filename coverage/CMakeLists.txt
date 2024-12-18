set(coverage_files
        coverage.c
        data.c
)

add_executable(coverage ${coverage_files})
target_link_libraries(coverage zinc)
set_target_properties(coverage
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)