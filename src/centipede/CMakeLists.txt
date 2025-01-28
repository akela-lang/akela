set(centipede_files
        token.c
        lex_data.c
        lex.c
        symbol.c
        value.c
        parse.c
        parse_data.c
        environment.c
        element.c
        enumerate.c
        ast.c
        parse_tools.c
        base.c
        update_types.c
        parse_stmts.c
        parse_expr.c
        parse_object.c
        build.c
        update_values.c
        check_value_types.c
        comp_unit.c
        module.c
        module_string.c
        module_file.c
        module_data.c
        module_import.c
        check_types.c
        parse_let.c
        comp_table.c
        check_parent.c
)

add_library(centipede ${centipede_files})
target_link_libraries(centipede json zinc)
set_target_properties(centipede
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
