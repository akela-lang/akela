cmake_minimum_required(VERSION 3.21)
project(alba)
set(source_files
    src/main.c
    src/scan.c
    src/result.c
)
add_executable(alba ${source_files})
