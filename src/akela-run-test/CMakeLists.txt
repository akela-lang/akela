set(run_files
        run.c
        parse.c
        data.c
        test_case.c
        type_info.c
)
add_executable(akela-run-test ${run_files})
target_link_libraries(akela-run-test akela-llvm ${llvm_libs} akela centipede cobble zinc)
set_target_properties(akela-run-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set(test_files
        test.c
        data.c
)

add_executable(akela-run-unit ${test_files})
target_link_libraries(akela-run-unit centipede cobble zinc)
set_target_properties(akela-run-unit
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)