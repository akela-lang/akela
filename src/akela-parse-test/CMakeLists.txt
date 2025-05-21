set(files
        test.c
        data.c
        parse.c
        compare.c
        parse_tools.c
)
add_library(akela-parse-test STATIC ${files})
target_link_libraries(akela-parse-test akela lava centipede zinc cobble)
set_target_properties(akela-parse-test
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
set(test_case_dir ${CMAKE_SOURCE_DIR}/src/akela-parse-test/test-cases)
FILE(WRITE ${CMAKE_BINARY_DIR}/bin/akela-parse-config.cent "\"${test_case_dir}\"")
