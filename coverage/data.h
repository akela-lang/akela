#ifndef COVERAGE_DATA_H
#define COVERAGE_DATA_H

#include "zinc/buffer.h"

typedef struct Coverage_file {
    struct buffer path;
    size_t number_of_lines;
    size_t number_covered;
    size_t number_not_covered;
    double coverage_percentage;
    struct Coverage_file* next;
    struct Coverage_file* prev;
} Coverage_file;

typedef struct Coverage_file_list {
    Coverage_file* head;
    Coverage_file* tail;
} Coverage_file_list;

typedef struct Coverage_library {
    struct buffer name;
    struct buffer path;
    Coverage_file_list coverage_files;
    struct Coverage_library* next;
    struct Coverage_library* prev;
} Coverage_library;

typedef struct Coverage_library_list
{
    Coverage_library* head;
    Coverage_library* tail;
} Coverage_library_list;

typedef struct Coverage_app {
    struct buffer data_path;
    Coverage_library_list libraries;
} Coverage_app;

#endif