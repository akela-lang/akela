set(run_files
        run.c
        parse.c
        data.c
        test_case.c
        type_info.c
)
add_library(akela-run-test STATIC ${run_files})
target_link_libraries(akela-run-test akela-llvm ${llvm_libs} akela centipede cobble lava zinc)
set_target_properties(akela-run-test
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)