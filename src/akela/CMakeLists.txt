# library
set(lib_files
        lex.c
        lex_tools.c
        token.c
        ast.c
        parse_stmts.c
        parse_expr.c
        parse_factor.c
        parse.c
        parse_tools.c
        parse_types.c
        symbol_table.c
        comp_unit.c
        comp_table.c
        code_gen.c
        symbol.c
        parse_data.c
        ast_to_cent.c
        environment.c
        type.c
        type_slots.c
        type_list.c
)

add_library(akela STATIC ${lib_files})
target_link_libraries(akela zinc)
set_target_properties(akela
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)