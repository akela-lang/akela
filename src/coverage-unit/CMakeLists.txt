set(coverage_unit_files
        test.c
        test_data.c
        test_parse.c
)
add_executable(coverage-unit ${coverage_unit_files})
target_link_libraries(coverage-unit coverage zinc ${coverage_lib})
set_target_properties(coverage-unit
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)