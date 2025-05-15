set(cobble_unit_files
        unit.c
        unit_ast_node.c
        test_compile.c
        test_compile_tools.c
        test_match.c
        test_match_tools.c
)

add_library(cobble-unit STATIC ${cobble_unit_files})
target_link_libraries(cobble-unit cobble zinc ${coverage_lib})
set_target_properties(cobble-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)