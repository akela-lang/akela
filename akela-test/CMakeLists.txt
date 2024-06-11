# akela-test
set(files
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
)
add_executable(akela-test ${files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(akela-test akela-llvm akela zinc gcov ${llvm_libs})
else()
    target_link_libraries(akela-test akela-llvm akela zinc ${llvm_libs})
endif()

# event loop
set(event_loop_files event_loop.c)
add_executable(event_loop ${event_loop_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(event_loop zinc gcov)
else()
    target_link_libraries(event_loop zinc)
endif()

# function_pointer
add_executable(function_pointer function_pointer.cpp)
target_link_libraries(function_pointer ${llvm_libs})

set_target_properties(akela-test event_loop function_pointer
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
