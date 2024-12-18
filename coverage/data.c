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