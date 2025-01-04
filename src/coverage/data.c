#include "data.h"
#include "zinc/memory.h"

void Cvr_file_init(Cvr_file *file)
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

void Cvr_file_create(Cvr_file **file)
{
    malloc_safe((void**)file, sizeof(Cvr_file));
    Cvr_file_init(*file);
}

void Cvr_file_destroy(Cvr_file *file)
{
    buffer_destroy(&file->name);
    buffer_destroy(&file->path);
    buffer_destroy(&file->source_path);
}

void Cvr_file_list_init(Cvr_file_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cvr_file_list_create(Cvr_file_list **list)
{
    malloc_safe((void**)list, sizeof(Cvr_file_list));
    Cvr_file_list_init(*list);
}

void Cvr_file_list_destroy(Cvr_file_list *list)
{
    Cvr_file *file = list->head;
    while (file) {
        Cvr_file* temp = file;
        file = file->next;
        Cvr_file_destroy(temp);
        free(temp);
    }
}

void Cvr_file_list_add(Cvr_file_list* list, Cvr_file *file)
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

void Cvr_file_list_add_sorted(Cvr_file_list* list, Cvr_file *file)
{
    if (list->head && list->tail) {
        Cvr_file* p = list->tail;
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

void Cvr_library_init(Cvr_library *lib)
{
    buffer_init(&lib->name);
    buffer_init(&lib->path);
    Cvr_file_list_init(&lib->files);
    lib->line_count = 0;
    lib->covered_count = 0;
    lib->not_covered_count = 0;
    lib->coverage_percentage = 0.0;
    lib->next = NULL;
    lib->prev = NULL;
}

void Cvr_library_create(Cvr_library **lib)
{
    malloc_safe((void**)lib, sizeof(Cvr_library));
    Cvr_library_init(*lib);
}

void Cvr_library_destroy(Cvr_library *lib)
{
    buffer_destroy(&lib->name);
    buffer_destroy(&lib->path);
    Cvr_file_list_destroy(&lib->files);
}

void Cvr_library_list_init(Cvr_library_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cvr_library_list_create(Cvr_library_list **list)
{
    malloc_safe((void**)list, sizeof(Cvr_library_list));
    Cvr_library_list_init(*list);
}

void Cvr_library_list_destroy(Cvr_library_list *list)
{
    Cvr_library *lib = list->head;
    while (lib) {
        Cvr_library* temp = lib;
        lib = lib->next;
        Cvr_library_destroy(temp);
        free(temp);
    }
}

void Cvr_library_list_add(Cvr_library_list *list, Cvr_library *lib)
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

void Cvr_library_list_add_sorted(Cvr_library_list* list, Cvr_library *lib)
{
    if (list->head && list->tail) {
        Cvr_library* p = list->tail;
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

void Cvr_app_init(Cvr_app *app)
{
    buffer_init(&app->data_path);
    Cvr_library_list_init(&app->libraries);
    app->line_count = 0;
    app->covered_count = 0;
    app->not_covered_count = 0;
    app->coverage_percentage = 0.0;
}

void Cvr_app_create(Cvr_app **app)
{
    malloc_safe((void**)app, sizeof(Cvr_app));
    Cvr_app_init(*app);
}

void Cvr_app_destroy(Cvr_app *app)
{
    buffer_destroy(&app->data_path);
    Cvr_library_list_destroy(&app->libraries);
}