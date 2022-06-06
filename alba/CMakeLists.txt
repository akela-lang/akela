
set(source_files
    alba.c
    scan_defer.c
    token.c
    result.c
    ustring.c
    memory.c
    defer.c
    dag.c
    parse_misc.c
    parse_rr.c
    parse_defer.c
    parse_al.c
    allocator.c
)
include_directories("../external/icu4c/include")
add_executable(alba ${source_files})
target_link_libraries(alba icudt icuin icuio icutest icutu icuuc)

file(GLOB icu_dll "${CMAKE_SOURCE_DIR}/external/icu4c/bin64/*.dll")
file(COPY ${icu_dll} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/examples/addition.alba" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
