#include "spec_error.h"
#include "memory.h"

void Zinc_spec_error_init(Zinc_spec_error* error)
{
    Zinc_string_init(&error->message);
    Zinc_location_init(&error->loc1);
    Zinc_location_init(&error->loc2);
    error->next = NULL;
    error->prev = NULL;
}

void Zinc_spec_error_create(Zinc_spec_error** error)
{
    Zinc_malloc_safe((void**)error, sizeof(Zinc_spec_error));
    Zinc_spec_error_init(*error);
}

void Zinc_spec_error_destroy(Zinc_spec_error* error)
{
    Zinc_string_destroy(&error->message);
}

void Zinc_spec_error_list_init(Zinc_spec_error_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Zinc_spec_error_list_create(Zinc_spec_error_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Zinc_spec_error_list));
    Zinc_spec_error_list_init(*list);
}

void Zinc_spec_error_list_add(Zinc_spec_error_list* list, Zinc_spec_error* error)
{
    if (list->head && list->tail) {
        error->prev = list->tail;
        list->tail->next = error;
        list->tail = error;
    } else {
        list->head = error;
        list->tail = error;
    }
}

void Zinc_spec_error_list_destroy(Zinc_spec_error_list* list)
{
    Zinc_spec_error* p = list->head;
    while (p) {
        Zinc_spec_error* temp = p;
        p = p->next;
        Zinc_spec_error_destroy(temp);
        free(temp);
    }
}

void Zinc_spec_error_list_set(
    Zinc_spec_error_list* errors,
    Zinc_location* loc1,
    Zinc_location* loc2,
    const char* fmt, ...)
{
    Zinc_spec_error* e = NULL;
    Zinc_spec_error_create(&e);
    e->loc1 = *loc1;
    e->loc2 = *loc2;

    va_list args;
    va_start(args, fmt);
    Zinc_string_add_vformat(&e->message, fmt, args);
    va_end(args);

    Zinc_spec_error_list_add(errors, e);
}