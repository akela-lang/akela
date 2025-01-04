set(coverage_test_files
        test.c
        test_data.c
        test_parse.c
)
add_executable(coverage-test ${coverage_test_files})
target_link_libraries(coverage-test coverage zinc ${coverage_lib})
set_target_properties(coverage-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)