#ifndef COVERAGE_DATA_H
#define COVERAGE_DATA_H

#include "zinc/buffer.h"

typedef struct Cvr_file {
    struct buffer name;
    struct buffer path;
    struct buffer source_path;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
    double coverage_percentage;
    struct Cvr_file* next;
    struct Cvr_file* prev;
} Cvr_file;

typedef struct Cvr_file_list {
    Cvr_file* head;
    Cvr_file* tail;
} Cvr_file_list;

typedef struct Cvr_library {
    struct buffer name;
    struct buffer path;
    Cvr_file_list files;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
    double coverage_percentage;
    struct Cvr_library* next;
    struct Cvr_library* prev;
} Cvr_library;

typedef struct Cvr_library_list
{
    Cvr_library* head;
    Cvr_library* tail;
} Cvr_library_list;

typedef struct Cvr_app {
    struct buffer data_path;
    Cvr_library_list libraries;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
    double coverage_percentage;
} Cvr_app;

void Cvr_file_init(Cvr_file *file);
void Cvr_file_create(Cvr_file **file);
void Cvr_file_destroy(Cvr_file *file);
void Cvr_file_list_init(Cvr_file_list *list);
void Cvr_file_list_create(Cvr_file_list **list);
void Cvr_file_list_destroy(Cvr_file_list *list);
void Cvr_file_list_add(Cvr_file_list* list, Cvr_file *file);
void Cvr_file_list_add_sorted(Cvr_file_list* list, Cvr_file *file);

void Cvr_library_init(Cvr_library *lib);
void Cvr_library_create(Cvr_library **lib);
void Cvr_library_destroy(Cvr_library *lib);
void Cvr_library_list_init(Cvr_library_list *list);
void Cvr_library_list_create(Cvr_library_list **list);
void Cvr_library_list_destroy(Cvr_library_list *list);
void Cvr_library_list_add(Cvr_library_list *list, Cvr_library *lib);
void Cvr_library_list_add_sorted(Cvr_library_list* list, Cvr_library *lib);

void Cvr_app_init(Cvr_app *app);
void Cvr_app_create(Cvr_app **app);
void Cvr_app_destroy(Cvr_app *app);

#endif