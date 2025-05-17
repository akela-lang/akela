set(centipede_unit_files
        unit.c
        test_lex.c
        test_lex_setup.c
        test_value.c
        unit_ast.c
        test_parse.c
        test_build.c
        test_check_value_types.c
        test_type.c
)

add_library(centipede-unit STATIC ${centipede_unit_files})
target_link_libraries(centipede-unit centipede zinc ${coverage_lib})
set_target_properties(centipede-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
