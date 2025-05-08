set(json_unit_files
        unit.c
        test_lex.c
        test_lex_tools.c
        unit_dom.c
        test_parse.c
        test_parse_tools.c
        test_stringify.c
)
add_library(json-unit ${json_unit_files})
target_link_libraries(json-unit json ${coverage_lib})
set_target_properties(json-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)