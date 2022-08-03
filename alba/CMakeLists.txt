
set(source_files
    alba.c
    scan.c
    token.c
    dag.c
    parse_stmts.c
    "parse_expr.c"
    parse_factor.c
    "source.c"
    parse.c
    parse_tools.c
    "input.c"
    os_win.c
    uconv.c
    parse_types.c
    lookahead_char.c
    hash.c
    symbol_table.c
)

if (WIN32)
    include_directories("../external/icu4c/include")

elseif(UNIX)
    include_directories("../external/icu/usr/local/include")

endif()

add_executable(alba ${source_files})

if(WIN32)

target_link_libraries(alba icudt icuin icuio icutest icutu icuuc zinc)

file(GLOB icu_dll "${CMAKE_SOURCE_DIR}/external/icu4c/bin64/*.dll")
file(COPY ${icu_dll} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/examples/addition.alba" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
        TARGET alba POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ZINC_DLL}
                ${CMAKE_CURRENT_BINARY_DIR}/zinc.dll
)

elseif(UNIX)

target_link_libraries(alba icuio icutest icutu icuuc icudata icutest zinc)

endif()
