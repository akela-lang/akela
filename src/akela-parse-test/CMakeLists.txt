set(files
        test.c
        data.c
        parse.c
        compare.c
        parse_tools.c
)
add_executable(akela-parse-test ${files})
target_link_libraries(akela-parse-test akela lava centipede zinc cobble)
set_target_properties(akela-parse-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)