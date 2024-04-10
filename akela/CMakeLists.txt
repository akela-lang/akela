
set(lib_files
        lex.c
        lex_tools.c
        token.c
        ast.c
        parse_stmts.c
        "parse_expr.c"
        parse_factor.c
        "source.c"
        parse.c
        parse_tools.c
        "input.c"
        os_win.c
        os_linux.c
        uconv.c
        parse_types.c
        lookahead_char.c
        lookahead_translator.c
        symbol_table.c
        type_def.c
        unit_test_compiler.c
        comp_unit.c
        comp_table.c
        code_gen.c
        code_gen_tools.c
        text_interface.c
)

set(bin_files
        akela.c
)

add_library(akela STATIC ${lib_files})
target_link_libraries(akela zinc icui18n icuio icutu icuuc icudata stdc++ m)

# executable
add_executable(akela-bin ${bin_files})
target_link_libraries(akela-bin akela zinc icui18n icuio icutu icuuc icudata stdc++ m ${llvm_libs})

SET_TARGET_PROPERTIES(akela-bin
  PROPERTIES OUTPUT_NAME akela)

add_custom_command(
        TARGET akela-bin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${EXAMPLE_ADDITION}
                ${AKELA_MATH}
                ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
        TARGET akela POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${EXAMPLE_ADDITION}
                ${AKELA_MATH}
                ${CMAKE_BINARY_DIR}/test
)
