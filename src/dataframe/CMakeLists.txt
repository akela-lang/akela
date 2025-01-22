set(dataframe_files
        csv_lex.c
        csv_parse.c
        csv_write.c
        data_frame.c
        field.c
        field_lex.c
        series.c
)

add_library(dataframe ${dataframe_files})
set_target_properties(dataframe
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)