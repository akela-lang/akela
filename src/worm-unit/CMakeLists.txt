set(worm_test_files
        test.c
        test_lex.c
)

add_executable(worm-unit ${worm_test_files})
target_link_libraries(worm-unit worm ${coverage_lib})
set_target_properties(worm-unit
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)