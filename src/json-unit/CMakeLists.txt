set(json_unit_files
        unit.c
        unit_lex.c
        unit_lex_tools.c
        unit_dom.c
        unit_parse.c
        unit_parse_tools.c
        test_stringify.c
)
add_library(json-unit ${json_unit_files})
target_link_libraries(json-unit json ${coverage_lib})
set_target_properties(json-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)