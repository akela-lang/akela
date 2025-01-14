#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdio.h>
#include "api.h"
#include "result.h"

#define BUFFER_CHUNK 64

typedef struct Zinc_string {
    size_t size;
    size_t buf_size;
    char* buf;
} Zinc_string;

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void Zinc_string_init(struct Zinc_string* bf);
ZINC_API void Zinc_string_create(struct Zinc_string** bf);
ZINC_API void Zinc_string_expand(struct Zinc_string* bf, size_t new_size);
ZINC_API void Zinc_string_add(struct Zinc_string* bf, const char* s, size_t num);
ZINC_API void Zinc_string_destroy(struct Zinc_string* bf);
ZINC_API void Zinc_string_free(struct Zinc_string* bf);
ZINC_API void Zinc_string_add_char(struct Zinc_string* bf, char c);
ZINC_API void Zinc_string_add_str(struct Zinc_string* bf, const char* str);
ZINC_API void Zinc_string_finish(struct Zinc_string* bf);
ZINC_API void Zinc_string_reset(struct Zinc_string* bf);
ZINC_API void Zinc_string_clear(struct Zinc_string* bf);
ZINC_API void Zinc_string_copy(struct Zinc_string* src, struct Zinc_string* dest);
ZINC_API void Zinc_string_create_str(struct Zinc_string* bf, char** a);
ZINC_API void Zinc_string_add_str2(const char* a, struct Zinc_string* bf);
ZINC_API enum result Zinc_next_char(struct Zinc_string* bf, size_t* pos, struct Zinc_string* bf2);
ZINC_API int Zinc_string_compare(struct Zinc_string* a, struct Zinc_string* b);
ZINC_API int Zinc_string_compare_str(struct Zinc_string* a, const char* b);
ZINC_API enum result Zinc_string_uslice(struct Zinc_string* src, struct Zinc_string* dest, size_t start, size_t end);
ZINC_API void Zinc_string_add_format(struct Zinc_string *bf, const char* fmt, ...);
ZINC_API int Zinc_string_order(struct Zinc_string* a, struct Zinc_string* b);
ZINC_API struct Zinc_string* Zinc_string_clone(struct Zinc_string* bf);

#ifdef __cplusplus
}
#endif

#endif
