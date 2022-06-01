
set(source_files
    alba.c
    scan.c
    result.c
    ustring.c
    memory.c
    defer.c
    dag.c
    old_parse.c
    parse.c
)
include_directories("../external/icu4c/include")
add_executable(alba ${source_files})
target_link_libraries(alba icudt icuin icuio icutest icutu icuuc)

file(GLOB icu_dll "${CMAKE_SOURCE_DIR}/external/icu4c/bin64/*.dll")
file(COPY ${icu_dll} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/examples/addition.alba" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
