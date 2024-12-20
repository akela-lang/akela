#ifndef COVERAGE_DATA_H
#define COVERAGE_DATA_H

#include "zinc/buffer.h"

typedef struct Cov_file {
    struct buffer name;
    struct buffer path;
    struct buffer source_path;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
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
    Cov_file_list files;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
    double coverage_percentage;
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
    size_t number_of_lines;
    size_t number_covered;
    size_t number_not_covered;
    double coverage_percentage;
} Cov_app;

void Cov_file_init(Cov_file *file);
void Cov_file_create(Cov_file **file);
void Cov_file_destroy(Cov_file *file);
void Cov_file_list_init(Cov_file_list *list);
void Cov_file_list_create(Cov_file_list **list);
void Cov_file_list_destroy(Cov_file_list *list);
void Cov_file_list_add(Cov_file_list* list, Cov_file *file);
void Cov_file_list_add_sorted(Cov_file_list* list, Cov_file *file);

void Cov_library_init(Cov_library *lib);
void Cov_library_create(Cov_library **lib);
void Cov_library_destroy(Cov_library *lib);
void Cov_library_list_init(Cov_library_list *list);
void Cov_library_list_create(Cov_library_list **list);
void Cov_library_list_destroy(Cov_library_list *list);
void Cov_library_list_add(Cov_library_list *list, Cov_library *lib);
void Cov_library_list_add_sorted(Cov_library_list* list, Cov_library *lib);

void Cov_app_init(Cov_app *app);
void Cov_app_create(Cov_app **app);
void Cov_app_destroy(Cov_app *app);

#endif