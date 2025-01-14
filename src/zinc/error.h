#ifndef ZINC_ERROR_H
#define ZINC_ERROR_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include "api.h"
#include "zstring.h"

typedef struct Zinc_location {
    size_t start_pos;
    size_t end_pos;
    size_t line;
    size_t col;
} Zinc_location;

typedef struct Zinc_error {
    Zinc_string message;
    Zinc_location loc;
    struct Zinc_error* next;
    struct Zinc_error* prev;
} Zinc_error;

typedef struct Zinc_error_list {
    Zinc_error* head;
    Zinc_error* tail;
} Zinc_error_list;

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void Zinc_error_init(Zinc_error* e);
ZINC_API void Zinc_error_create(Zinc_error** e);
ZINC_API void Zinc_error_destroy(Zinc_error* e);
ZINC_API void Zinc_error_list_init(Zinc_error_list* el);
ZINC_API void Zinc_error_list_create(Zinc_error_list** el);
ZINC_API void Zinc_error_list_add(Zinc_error_list *el, Zinc_error* e);
ZINC_API void Zinc_error_list_destroy(Zinc_error_list* el);
ZINC_API bool Zinc_error_list_set(Zinc_error_list *el, Zinc_location* loc, const char* fmt, ...);
ZINC_API void Zinc_error_list_print(Zinc_error_list* el);
ZINC_API void Zinc_location_init(Zinc_location* loc);
ZINC_API void Zinc_location_create(Zinc_location** loc);
ZINC_API const char* Zinc_plural(size_t number);
ZINC_API void Zinc_location_combine(Zinc_location* p, Zinc_location* c);

#ifdef __cplusplus
}
#endif

#endif
