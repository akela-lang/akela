#ifndef ZINC_SPEC_ERROR_H
#define ZINC_SPEC_ERROR_H

#include "error.h"
#include "zstring.h"

typedef struct Zinc_spec_error {
    Zinc_string message;
    Zinc_location loc1;
    Zinc_location loc2;
    struct Zinc_spec_error *next;
    struct Zinc_spec_error *prev;
} Zinc_spec_error;

typedef struct Zinc_spec_error_list {
    Zinc_spec_error *head;
    Zinc_spec_error *tail;
} Zinc_spec_error_list;

void Zinc_spec_error_init(Zinc_spec_error* error);
void Zinc_spec_error_create(Zinc_spec_error** error);
void Zinc_spec_error_destroy(Zinc_spec_error* error);
void Zinc_spec_error_list_init(Zinc_spec_error_list* list);
void Zinc_spec_error_list_create(Zinc_spec_error_list** list);
void Zinc_spec_error_list_add(Zinc_spec_error_list* list, Zinc_spec_error* error);
void Zinc_spec_error_list_destroy(Zinc_spec_error_list* list);
void Zinc_spec_error_list_set(
    Zinc_spec_error_list* errors,
    Zinc_location* loc1,
    Zinc_location* loc2,
    const char* fmt, ...);

#endif