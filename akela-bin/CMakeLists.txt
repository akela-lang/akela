# executable
set(bin_files
    akela.c
)
add_executable(akela-bin ${bin_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(akela-bin akela zinc gcov ${llvm_libs})
else()
    target_link_libraries(akela-bin akela zinc ${llvm_libs})
endif()

set_target_properties(akela-bin
        PROPERTIES
        OUTPUT_NAME akela
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_custom_command(
        TARGET akela-bin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${EXAMPLE_ADDITION}
        ${AKELA_MATH}
        ${CMAKE_BINARY_DIR}/bin
)