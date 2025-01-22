# files
set(SOURCE_FILES
        test.c
        test_result.c
        test_buffer.c
        test_memory.c
        test_unit_test.c
        test_utf8.c
        test_hash.c
        test_os_unix.c
        test_datetime.c
        test_piece.c
        test_buffer_list.c
        test_error.c
        test_list.c
        test_vector.c
        test_input_char_string.c
        test_input_char_file.c
        test_input_unicode_string.c
        test_input_unicode_file.c
        test_hash_map_size_t.c
        test_unicode.c
        test_spec_error.c
)

add_executable(zinc-unit ${SOURCE_FILES})
target_link_libraries(zinc-unit zinc ${coverage_lib})
set_target_properties(zinc-unit
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)