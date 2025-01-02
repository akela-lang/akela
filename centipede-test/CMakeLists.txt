set(centipede_test_files
        test.c
        test_lex.c
        test_lex_setup.c
        test_value.c
        test_ast.c
        test_parse.c
        test_parse_tools.c
        test_build.c
        test_build_tools.c
        test_check_types.c
)

add_executable(centipede-test ${centipede_test_files})
target_link_libraries(centipede-test centipede zinc)
set_target_properties(centipede-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)