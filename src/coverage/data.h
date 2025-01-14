#ifndef COVERAGE_DATA_H
#define COVERAGE_DATA_H

#include "zinc/zstring.h"

typedef struct Cover_file {
    struct Zinc_string name;
    struct Zinc_string path;
    struct Zinc_string source_path;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
    double coverage_percentage;
    struct Cover_file* next;
    struct Cover_file* prev;
} Cover_file;

typedef struct Cover_file_list {
    Cover_file* head;
    Cover_file* tail;
} Cover_file_list;

typedef struct Cover_library {
    struct Zinc_string name;
    struct Zinc_string path;
    Cover_file_list files;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
    double coverage_percentage;
    struct Cover_library* next;
    struct Cover_library* prev;
} Cover_library;

typedef struct Cover_library_list
{
    Cover_library* head;
    Cover_library* tail;
} Cover_library_list;

typedef struct Cover_app {
    struct Zinc_string data_path;
    Cover_library_list libraries;
    size_t line_count;
    size_t covered_count;
    size_t not_covered_count;
    double coverage_percentage;
} Cover_app;

void Cover_file_init(Cover_file *file);
void Cover_file_create(Cover_file **file);
void Cover_file_destroy(Cover_file *file);
void Cover_file_list_init(Cover_file_list *list);
void Cover_file_list_create(Cover_file_list **list);
void Cover_file_list_destroy(Cover_file_list *list);
void Cover_file_list_add(Cover_file_list* list, Cover_file *file);
void Cover_file_list_add_sorted(Cover_file_list* list, Cover_file *file);

void Cover_library_init(Cover_library *lib);
void Cover_library_create(Cover_library **lib);
void Cover_library_destroy(Cover_library *lib);
void Cover_library_list_init(Cover_library_list *list);
void Cover_library_list_create(Cover_library_list **list);
void Cover_library_list_destroy(Cover_library_list *list);
void Cover_library_list_add(Cover_library_list *list, Cover_library *lib);
void Cover_library_list_add_sorted(Cover_library_list* list, Cover_library *lib);

void Cover_app_init(Cover_app *app);
void Cover_app_create(Cover_app **app);
void Cover_app_destroy(Cover_app *app);

#endif