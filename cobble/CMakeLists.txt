set (cobble_files
        Ast_node.c
        compile.c
        token.c
        compile_data.c
        compile_tools.c
        match.c
        match_tools.c
)
add_library(cobble ${cobble_files})
target_link_libraries(cobble json zinc icudata icui18n icuio icutu icuuc)