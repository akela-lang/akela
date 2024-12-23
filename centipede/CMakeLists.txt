set(centipede_files
        token.c
        lex_data.c
        lex.c
        symbol.c
        value.c
        lex_tools.c
)

add_library(centipede ${centipede_files})