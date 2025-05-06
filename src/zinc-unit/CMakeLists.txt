# files
set(SOURCE_FILES
        unit_result.c
        unit_string.c
        unit_memory.c
        unit_utf8.c
        unit_hash_map_string.c
        unit_os_unix.c
        unit_datetime.c
        unit_piece.c
        unit_string_list.c
        unit_error.c
        unit_list.c
        unit_vector.c
        unit_input_char_string.c
        unit_input_char_file.c
        unit_input_unicode_string.c
        unit_input_unicode_file.c
        unit_hash_map_size_t.c
        unit_unicode.c
        unit_spec_error.c
        unit_priority_queue.c
        unit_dbl_vector.c
        unit_string_slice.c
        unit_test.c
        unit.c
)

add_library(zinc-unit STATIC ${SOURCE_FILES})
target_link_libraries(zinc-unit zinc ${coverage_lib})
set_target_properties(zinc-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)