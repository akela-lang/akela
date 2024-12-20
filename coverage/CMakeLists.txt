set(coverage_files
        data.c
        parse.c
)
add_library(coverage ${coverage_files})
target_link_libraries(coverage zinc)