
# files
set(SOURCE_FILES
        unit_test.c
        buffer.c
        memory.c
        result.c
        utf8.c
        hash.c
        os_unix.c
        os_win.c
        datetime.c
        piece.c
        buffer_list.c
        error.c
        error_unit_test.c
        list.c
        vector.c
        series.c
        data_frame.c
        field.c
        field_lex.c
        input_char_string.c
        csv_lex.c
        csv_parse.c
        csv_write.c
        input_char.c
        input_char_file.c
        input_unicode.c
        input_unicode_string.c
        input_unicode_file.c
)

add_definitions(-DZINC_EXPORT)
add_library(zinc STATIC ${SOURCE_FILES})
set_target_properties(zinc
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
