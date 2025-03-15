set(files
        token.c
        lex.c
        lex_data.c
        section.c
)

add_library(lava ${files})
target_link_libraries(lava zinc)
set_target_properties(lava
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)