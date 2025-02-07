set(centipede_unit_files
        test.c
        test_lex.c
        test_lex_setup.c
        test_value.c
        test_ast.c
        test_parse.c
        test_parse_tools.c
        test_build.c
        test_check_value_types.c
        test_type.c
)

add_executable(centipede-unit ${centipede_unit_files})
target_link_libraries(centipede-unit centipede zinc ${coverage_lib})
set_target_properties(centipede-unit
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
