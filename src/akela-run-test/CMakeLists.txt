set(files
        test.c
        parse.c
        data.c
)
add_executable(akela-run-test ${files})
target_link_libraries(akela-run-test cobble zinc)
set_target_properties(akela-run-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)