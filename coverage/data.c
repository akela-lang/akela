#include "data.h"
#include "zinc/memory.h"

void Cov_file_init(Cov_file *file)
{
    buffer_init(&file->path);
    file->number_of_lines = 0;
    file->number_covered = 0;
    file->number_not_covered = 0;
    file->coverage_percentage = 0.0;
    file->next = NULL;
    file->prev = NULL;
}

void Cov_file_create(Cov_file **file)
{
    malloc_safe((void**)file, sizeof(Cov_file));
    Cov_file_init(*file);
}

void Cov_file_destroy(Cov_file *file)
{
    buffer_destroy(&file->path);
}

void Cov_file_list_init(Cov_file_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cov_file_list_create(Cov_file_list **list)
{
    malloc_safe((void**)list, sizeof(Cov_file_list));
    Cov_file_list_init(*list);
}

void Cov_file_list_add(Cov_file_list* list, Cov_file *file)
{
    if (list->head && list->tail) {
        list->tail->next = file;
        file->prev = list->tail;
        list->tail = file;
    } else {
        list->head = file;
        list->tail = file;
    }
}

void Cov_file_list_destroy(Cov_file_list *list)
{
    Cov_file *file = list->head;
    while (file) {
        Cov_file* temp = file;
        file = file->next;
        Cov_file_destroy(temp);
        free(temp);
    }
}

void Cov_library_init(Cov_library *lib)
{
    buffer_init(&lib->name);
    buffer_init(&lib->path);
    Cov_file_list_init(&lib->coverage_files);
    lib->number_of_lines = 0;
    lib->number_covered = 0;
    lib->number_not_covered = 0;
    lib->coverage_percentage = 0.0;
    lib->next = NULL;
    lib->prev = NULL;
}

void Cov_library_create(Cov_library **lib)
{
    malloc_safe((void**)lib, sizeof(Cov_library));
    Cov_library_init(*lib);
}

void Cov_library_destroy(Cov_library *lib)
{
    buffer_destroy(&lib->name);
    buffer_destroy(&lib->path);
    Cov_file_list_destroy(&lib->coverage_files);
}

void Cov_library_list_init(Cov_library_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cov_library_list_create(Cov_library_list **list)
{
    malloc_safe((void**)list, sizeof(Cov_library_list));
    Cov_library_list_init(*list);
}

void Cov_library_list_destroy(Cov_library_list *list)
{
    Cov_library *lib = list->head;
    while (lib) {
        Cov_library* temp = lib;
        lib = lib->next;
        Cov_library_destroy(temp);
        free(temp);
    }
}

void Cov_library_list_add(Cov_library_list *list, Cov_library *lib)
{
    if (list->head && list->tail) {
        list->tail->next = lib;
        lib->prev = list->tail;
        list->tail = lib;
    } else {
        list->head = lib;
        list->tail = lib;
    }
}

void Cov_app_init(Cov_app *app)
{
    buffer_init(&app->data_path);
    Cov_library_list_init(&app->libraries);
    app->number_of_lines = 0;
    app->number_covered = 0;
    app->number_not_covered = 0;
    app->coverage_percentage = 0.0;
}

void Cov_app_create(Cov_app **app)
{
    malloc_safe((void**)app, sizeof(Cov_app));
    Cov_app_init(*app);
}

void Cov_app_destroy(Cov_app *app)
{
    buffer_destroy(&app->data_path);
    Cov_library_list_destroy(&app->libraries);
}