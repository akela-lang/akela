# executable
set(bin_files
    akela.c
)
add_executable(akela-bin ${bin_files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(akela-bin akela-llvm akela zinc gcov ${llvm_libs})
else()
    target_link_libraries(akela-bin akela-llvm akela zinc ${llvm_libs})
endif()

set_target_properties(akela-bin
        PROPERTIES
        OUTPUT_NAME akela
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)