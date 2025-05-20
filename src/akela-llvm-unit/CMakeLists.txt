# test
set(akela_llvm_unit_files
        unit.c
        unit_cg_tools.c
        unit_literal.c
        test_variable.c
        unit_operator.c
        unit_stmts.c
        unit_function.c
        unit_array.c
        test_struct.c
)
add_library(akela-llvm-unit STATIC ${akela_llvm_unit_files})
target_link_libraries(akela-llvm-unit akela-llvm akela zinc ${coverage_lib} ${llvm_libs})
set_target_properties(akela-llvm-unit
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# function_pointer
add_executable(function_pointer function_pointer.cpp)
target_link_libraries(function_pointer ${llvm_libs} ${coverage_lib})

add_executable(llvm_abort abort.c)
target_link_libraries(llvm_abort ${coverage_lib})

set_target_properties(function_pointer llvm_abort
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
