set(worm_files
        lex.c
        token.c
        lex_data.c
        ast.c
        parse.c
        parse_data.c
        parse_tools.c
        parse_stmts.c
        symbol.c
)

add_library(worm ${worm_files})
target_link_libraries(worm centipede zinc)
set_target_properties(worm
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
