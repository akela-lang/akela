set(dataframe_unit_files
        unit.c
        unit_csv_lex.c
        test_csv_parse.c
        test_csv_parse_tools.c
        test_csv_write.c
        test_data_frame.c
        test_field_lex.c
)

add_library(dataframe-unit STATIC ${dataframe_unit_files})
target_link_libraries(dataframe-unit dataframe zinc ${coverage_lib})
set_target_properties(dataframe-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)