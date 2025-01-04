#ifndef ZINC_ERROR_H
#define ZINC_ERROR_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include "api.h"
#include "buffer.h"

struct location {
    size_t start_pos;
    size_t end_pos;
    size_t line;
    size_t col;
};

struct error {
    struct buffer message;
    struct location loc;
    struct error* next;
    struct error* prev;
};

struct error_list {
    struct error* head;
    struct error* tail;
};

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void error_init(struct error* e);
ZINC_API void error_create(struct error** e);
ZINC_API void error_destroy(struct error* e);
ZINC_API void error_list_init(struct error_list* el);
ZINC_API void error_list_create(struct error_list** el);
ZINC_API void error_list_add(struct error_list *el, struct error* e);
ZINC_API void error_list_destroy(struct error_list* el);
ZINC_API bool error_list_set(struct error_list *el, struct location* loc, const char* fmt, ...);
ZINC_API void error_list_print(struct error_list* el);
ZINC_API void location_init(struct location* loc);
ZINC_API void location_create(struct location** loc);
ZINC_API const char* plural(size_t number);
ZINC_API void location_combine(struct location* p, struct location* c);

#ifdef __cplusplus
}
#endif

#endif
