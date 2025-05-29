
# files
set(SOURCE_FILES
        zstring.c
        memory.c
        result.c
        utf8.c
        hash_map_string.c
        os_unix.c
        os_win.c
        datetime.c
        piece.c
        string_list.c
        error.c
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
        spec_error.c
        priority_queue.c
        dbl_vector.c
        fs.c
        string_slice.c
        input_bounds.c
        test.c
        expect.c
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
