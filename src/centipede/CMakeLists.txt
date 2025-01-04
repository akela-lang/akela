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
        parse_object_stmts.c
        build.c
        update_values.c
        check_types.c
        comp_unit.c
)

add_library(centipede ${centipede_files})
target_link_libraries(centipede json zinc)