#include "data.h"
#include "zinc/memory.h"

void Cover_file_init(Cover_file *file)
{
    Zinc_string_init(&file->name);
    Zinc_string_init(&file->path);
    Zinc_string_init(&file->source_path);
    file->line_count = 0;
    file->covered_count = 0;
    file->not_covered_count = 0;
    file->coverage_percentage = 0.0;
    file->next = NULL;
    file->prev = NULL;
}

void Cover_file_create(Cover_file **file)
{
    Zinc_malloc_safe((void**)file, sizeof(Cover_file));
    Cover_file_init(*file);
}

void Cover_file_destroy(Cover_file *file)
{
    Zinc_string_destroy(&file->name);
    Zinc_string_destroy(&file->path);
    Zinc_string_destroy(&file->source_path);
}

void Cover_file_list_init(Cover_file_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cover_file_list_create(Cover_file_list **list)
{
    Zinc_malloc_safe((void**)list, sizeof(Cover_file_list));
    Cover_file_list_init(*list);
}

void Cover_file_list_destroy(Cover_file_list *list)
{
    Cover_file *file = list->head;
    while (file) {
        Cover_file* temp = file;
        file = file->next;
        Cover_file_destroy(temp);
        free(temp);
    }
}

void Cover_file_list_add(Cover_file_list* list, Cover_file *file)
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

void Cover_file_list_add_sorted(Cover_file_list* list, Cover_file *file)
{
    if (list->head && list->tail) {
        Cover_file* p = list->tail;
        while (p) {
            int order = Zinc_string_order(&p->name, &file->name);
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

void Cover_library_init(Cover_library *lib)
{
    Zinc_string_init(&lib->name);
    Zinc_string_init(&lib->path);
    Cover_file_list_init(&lib->files);
    lib->line_count = 0;
    lib->covered_count = 0;
    lib->not_covered_count = 0;
    lib->coverage_percentage = 0.0;
    lib->next = NULL;
    lib->prev = NULL;
}

void Cover_library_create(Cover_library **lib)
{
    Zinc_malloc_safe((void**)lib, sizeof(Cover_library));
    Cover_library_init(*lib);
}

void Cover_library_destroy(Cover_library *lib)
{
    Zinc_string_destroy(&lib->name);
    Zinc_string_destroy(&lib->path);
    Cover_file_list_destroy(&lib->files);
}

void Cover_library_list_init(Cover_library_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cover_library_list_create(Cover_library_list **list)
{
    Zinc_malloc_safe((void**)list, sizeof(Cover_library_list));
    Cover_library_list_init(*list);
}

void Cover_library_list_destroy(Cover_library_list *list)
{
    Cover_library *lib = list->head;
    while (lib) {
        Cover_library* temp = lib;
        lib = lib->next;
        Cover_library_destroy(temp);
        free(temp);
    }
}

void Cover_library_list_add(Cover_library_list *list, Cover_library *lib)
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

void Cover_library_list_add_sorted(Cover_library_list* list, Cover_library *lib)
{
    if (list->head && list->tail) {
        Cover_library* p = list->tail;
        while (p) {
            int order = Zinc_string_order(&p->name, &lib->name);
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

void Cover_app_init(Cover_app *app)
{
    Zinc_string_init(&app->data_path);
    Cover_library_list_init(&app->libraries);
    app->line_count = 0;
    app->covered_count = 0;
    app->not_covered_count = 0;
    app->coverage_percentage = 0.0;
}

void Cover_app_create(Cover_app **app)
{
    Zinc_malloc_safe((void**)app, sizeof(Cover_app));
    Cover_app_init(*app);
}

void Cover_app_destroy(Cover_app *app)
{
    Zinc_string_destroy(&app->data_path);
    Cover_library_list_destroy(&app->libraries);
}