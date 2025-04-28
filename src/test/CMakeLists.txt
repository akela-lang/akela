set(files main.c)
add_executable(test ${files})
target_link_libraries(test zinc-unit)
