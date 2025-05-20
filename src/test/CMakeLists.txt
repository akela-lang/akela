set(files main.c)
add_executable(test ${files})
target_link_libraries(test
        zinc-unit
        lava-unit
        json-unit
        dataframe-unit
        coverage-unit
        cobble-unit
        centipede-unit
        akela-unit
        akela-llvm-unit
)
set_target_properties(test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)