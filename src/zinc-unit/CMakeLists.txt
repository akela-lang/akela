# files
set(SOURCE_FILES
        test_result.c
        unit_string.c
        test_memory.c
        test_unit_test.c
        test_utf8.c
        test_hash_map_string.c
        test_os_unix.c
        test_datetime.c
        test_piece.c
        test_string_list.c
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
        test_priority_queue.c
        test_dbl_vector.c
        test_string_slice.c
        test_test.c
        unit.c
)

add_library(zinc-unit STATIC ${SOURCE_FILES})
target_link_libraries(zinc-unit zinc ${coverage_lib})
set_target_properties(zinc-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_executable(zinc-unit-bin test.c)
target_link_libraries(zinc-unit-bin zinc zinc-unit)
set_target_properties(zinc-unit-bin
        PROPERTIES
        OUTPUT_NAME zinc-unit
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)