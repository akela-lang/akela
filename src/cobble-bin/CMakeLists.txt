set(files
        cobble.c
)
add_executable(cobble-bin ${files})
if(${COVERAGE} MATCHES On)
    target_link_libraries(cobble-bin cobble zinc gcov ${llvm_libs})
else()
    target_link_libraries(cobble-bin cobble zinc ${llvm_libs})
endif()

set_target_properties(cobble-bin
        PROPERTIES
        OUTPUT_NAME cobble
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)