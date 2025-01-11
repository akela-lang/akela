set(files
        test.c
)
add_executable(akela-parse-test ${files})
target_link_libraries(akela-parse-test akela centipede)
set_target_properties(akela-parse-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)