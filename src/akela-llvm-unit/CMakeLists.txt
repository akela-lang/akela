# test
set(akela_llvm_unit_files
        unit.c
        unit_cg_tools.c
)
add_library(akela-llvm-unit STATIC ${akela_llvm_unit_files})
target_link_libraries(akela-llvm-unit akela-llvm akela zinc ${coverage_lib} ${llvm_libs})
set_target_properties(akela-llvm-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)