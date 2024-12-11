set(json_files
        token.c
        lex.c
        lex_tools.c
)
add_library(json ${json_files})
target_link_libraries(json zinc icudata icui18n icuio icutu icuuc)