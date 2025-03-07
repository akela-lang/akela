set(json_files
        token.c
        lex.c
        lex_tools.c
        dom.c
        parse.c
        parse_tools.c
        general.c
        stringify.c
)
add_library(json ${json_files})
target_link_libraries(json zinc ${ICU_LIBS})
set_target_properties(json
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)