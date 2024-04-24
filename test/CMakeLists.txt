set(test_files
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
        test_code_gen.c
        test_code_gen_tools.c
)

add_executable(test ${test_files})
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    target_link_libraries(test akela zinc gcov ${llvm_libs})
else()
    target_link_libraries(test akela zinc ${llvm_libs})
endif()


set(event_loop_files event_loop.c)
add_executable(event_loop ${event_loop_files})
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    target_link_libraries(event_loop akela zinc gcov)
else()
    target_link_libraries(event_loop akela zinc)
endif()

