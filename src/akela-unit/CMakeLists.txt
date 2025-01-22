# akela-unit
set(akela_unit_files
        test.c
        test_ast.c
        test_parse.c
        test_parse_factor.c
        test_parse_expr.c
        test_parse_stmts.c
        test_lex_setup.c
        test_lex.c
        test_lex_number.c
        test_lex_tools.c
        test_token.c
        test_symbol_table.c
        test_parse_types.c
        test_type.c
        test_comp_unit.c
        test_comp_table.c
        test_parse_function.c
        test_parse_struct.c
)
add_executable(akela-unit ${akela_unit_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(akela-unit akela zinc ${converage_lib})
else()
    target_link_libraries(akela-unit akela zinc ${converage_lib})
endif()

# event loop
set(event_loop_files event_loop.c)
add_executable(event_loop ${event_loop_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(event_loop zinc gcov)
else()
    target_link_libraries(event_loop zinc)
endif()

set_target_properties(akela-unit event_loop
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)