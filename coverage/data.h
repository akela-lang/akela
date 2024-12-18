#ifndef COVERAGE_DATA_H
#define COVERAGE_DATA_H

#include "zinc/buffer.h"

typedef struct Cov_file {
    struct buffer path;
    size_t number_of_lines;
    size_t number_covered;
    size_t number_not_covered;
    double coverage_percentage;
    struct Cov_file* next;
    struct Cov_file* prev;
} Cov_file;

typedef struct Cov_file_list {
    Cov_file* head;
    Cov_file* tail;
} Cov_file_list;

typedef struct Cov_library {
    struct buffer name;
    struct buffer path;
    Cov_file_list coverage_files;
    struct Cov_library* next;
    struct Cov_library* prev;
} Cov_library;

typedef struct Cov_library_list
{
    Cov_library* head;
    Cov_library* tail;
} Cov_library_list;

typedef struct Cov_app {
    struct buffer data_path;
    Cov_library_list libraries;
} Cov_app;

#endif