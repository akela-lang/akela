set(centipede_test_files
        test.c
        test_lex.c
)

add_executable(centipede-test ${centipede_test_files})
target_link_libraries(centipede-test centipede zinc)
set_target_properties(centipede-test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)