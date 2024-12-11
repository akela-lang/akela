set(json_files
        token.c
)
add_library(json ${json_files})
target_link_libraries(json zinc)