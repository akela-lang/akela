set(centipede_files
        token.c
        lex_data.c
        lex.c
        symbol.c
        value.c
        lex_tools.c
        parse.c
        parse_data.c
        environment.c
        element.c
        enumerate.c
        ast.c
        parse_tools.c
        base.c
)

add_library(centipede ${centipede_files})
target_link_libraries(centipede json zinc)