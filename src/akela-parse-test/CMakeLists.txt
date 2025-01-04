set(files
        test.c
)
add_executable(akela-parse-test ${files})
target_link_libraries(akela-parse-test akela centipede)