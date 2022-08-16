
set(lib_files
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
    lookahead_translator.c
    hash.c
    symbol_table.c
)

set(bin_files
    alba.c
)

if (WIN32)
    include_directories("../external/icu4c/include")

elseif(UNIX)
    include_directories("../external/icu/usr/local/include")

endif()

# library
add_definitions(-DALBA_EXPORT)
add_library(alba-lib SHARED ${lib_files})
target_link_libraries(alba-lib icudt icuin icuio icutest icutu icuuc zinc)
SET_TARGET_PROPERTIES(alba-lib
  PROPERTIES OUTPUT_NAME alba)

# executable
add_executable(alba-bin ${bin_files})
target_link_libraries(alba-bin alba-lib icudt icuin icuio icutest icutu icuuc zinc)
SET_TARGET_PROPERTIES(alba-bin
  PROPERTIES OUTPUT_NAME alba)

if(WIN32)

elseif(UNIX)

target_link_libraries(alba-lib icuio icutest icutu icuuc icudata icutest zinc)

endif()
