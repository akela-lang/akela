set(coverage_test_files
        test.c
        test_data.c
)
add_executable(coverage-test ${coverage_test_files})
target_link_libraries(coverage-test coverage zinc)