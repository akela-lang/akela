
# files
set(SOURCE_FILES
        unit_test.c
        zstring.c
        memory.c
        result.c
        utf8.c
        hash.c
        os_unix.c
        os_win.c
        datetime.c
        piece.c
        string_list.c
        error.c
        error_unit_test.c
        list.c
        vector.c
        input_char_string.c
        input_char.c
        input_char_file.c
        input_unicode.c
        input_unicode_string.c
        input_unicode_file.c
        hash_map_size_t.c
        unicode.c
)

add_definitions(-DZINC_EXPORT)
add_library(zinc STATIC ${SOURCE_FILES})

if(${COVERAGE} MATCHES On)
    target_link_libraries(zinc gcov)
endif ()

set_target_properties(zinc
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
