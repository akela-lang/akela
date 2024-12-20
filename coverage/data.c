#include "data.h"
#include "zinc/memory.h"

void Cov_file_init(Cov_file *file)
{
    buffer_init(&file->name);
    buffer_init(&file->path);
    buffer_init(&file->source_path);
    file->line_count = 0;
    file->covered_count = 0;
    file->not_covered_count = 0;
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
    buffer_destroy(&file->name);
    buffer_destroy(&file->path);
    buffer_destroy(&file->source_path);
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

void Cov_file_list_add_sorted(Cov_file_list* list, Cov_file *file)
{
    if (list->head && list->tail) {
        Cov_file* p = list->tail;
        while (p) {
            int order = buffer_order(&p->name, &file->name);
            if (order == -1 || order == 0) {
                break;
            }
            p = p->prev;
        }

        if (p) {
            if (p->next) {
                file->next = p->next;
                p->next->prev = file;
            }
            p->next = file;
            file->prev = p;
            if (list->tail == p) {
                list->tail = file;
            }
        } else {
            file->next = list->head;
            list->head->prev = file;
            list->head = file;
        }
    } else {
        list->head = file;
        list->tail = file;
    }
}

void Cov_library_init(Cov_library *lib)
{
    buffer_init(&lib->name);
    buffer_init(&lib->path);
    Cov_file_list_init(&lib->files);
    lib->line_count = 0;
    lib->covered_count = 0;
    lib->not_covered_count = 0;
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
    Cov_file_list_destroy(&lib->files);
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

void Cov_library_list_add_sorted(Cov_library_list* list, Cov_library *lib)
{
    if (list->head && list->tail) {
        Cov_library* p = list->tail;
        while (p) {
            int order = buffer_order(&p->name, &lib->name);
            if (order == -1 || order == 0) {
                break;
            }
            p = p->prev;
        }

        if (p) {
            if (p->next) {
                lib->next = p->next;
                p->next->prev = lib;
            }
            p->next = lib;
            lib->prev = p;
            if (list->tail == p) {
                list->tail = lib;
            }
        } else {
            lib->next = list->head;
            list->head->prev = lib;
            list->head = lib;
        }
    } else {
        list->head = lib;
        list->tail = lib;
    }
}

void Cov_app_init(Cov_app *app)
{
    buffer_init(&app->data_path);
    Cov_library_list_init(&app->libraries);
    app->line_count = 0;
    app->covered_count = 0;
    app->not_covered_count = 0;
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