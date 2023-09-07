set(test_files
        test.c
        test_ast.c
        test_parse.c
        test_parse_factor.c
        test_parse_expr.c
        test_parse_stmts.c
        test_scan_setup.c
        test_scan.c
        test_scan_number.c
        test_input.c
        test_token.c
        test_lookahead_translator.c
        test_lookahead_char.c
        test_symbol_table.c
        test_parse_types.c
        test_type.c
        test_comp_unit.c
        test_comp_table.c
        test_code_gen.c
        test_code_gen_tools.c
)

#include_directories("..")

add_executable(test ${test_files})

target_link_libraries(test akela zinc icui18n icuio icutu icuuc icudata stdc++ m ${llvm_libs})
