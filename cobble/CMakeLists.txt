set (cobble_files
        Ast_node.c
        compile.c
        token.c
        compile_data.c
        compile_tools.c
        match.c
        match_tools.c
        unit_test_string_slice.c)
add_library(cobble ${cobble_files})
if (${ICU_LIB} MATCHES ENABLE)
    target_link_libraries(cobble zinc icudata icui18n icuio icutu icuuc)
else()
    target_link_libraries(cobble zinc)
endif()
