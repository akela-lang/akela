#ifndef ZINC_STRING_H
#define ZINC_STRING_H

#include <stdio.h>
#include "api.h"
#include "result.h"
#include <stdbool.h>

#define BUFFER_CHUNK 64

typedef struct Zinc_string {
    size_t size;
    size_t buf_size;
    char* buf;
} Zinc_string;

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void Zinc_string_init(Zinc_string* bf);
ZINC_API void Zinc_string_create(Zinc_string** bf);
ZINC_API void Zinc_string_expand(Zinc_string* bf, size_t new_size);
ZINC_API void Zinc_string_add(Zinc_string* bf, const char* s, size_t num);
ZINC_API void Zinc_string_destroy(Zinc_string* bf);
ZINC_API void Zinc_string_free(Zinc_string* bf);
ZINC_API void Zinc_string_add_char(Zinc_string* bf, char c);
ZINC_API void Zinc_string_add_str(Zinc_string* bf, const char* str);
ZINC_API void Zinc_string_finish(Zinc_string* bf);
ZINC_API char* Zinc_string_c_str(Zinc_string* bf);
ZINC_API void Zinc_string_reset(Zinc_string* bf);
ZINC_API void Zinc_string_clear(Zinc_string* bf);
ZINC_API void Zinc_string_copy(Zinc_string* src, Zinc_string* dest);
ZINC_API void Zinc_string_create_str(Zinc_string* bf, char** a);
ZINC_API void Zinc_string_add_str2(const char* a, Zinc_string* bf);
ZINC_API void Zinc_string_add_string(Zinc_string* string, Zinc_string* string2);
ZINC_API Zinc_result Zinc_string_next(Zinc_string* text, size_t* pos, char c[5], int* num, bool* done);
ZINC_API int Zinc_string_compare(Zinc_string* a, Zinc_string* b);
ZINC_API int Zinc_string_compare_str(Zinc_string* a, const char* b);
ZINC_API Zinc_result Zinc_string_uslice(Zinc_string* src, Zinc_string* dest, size_t start, size_t end);
ZINC_API void Zinc_string_add_format(Zinc_string *bf, const char* fmt, ...);
ZINC_API int Zinc_string_order(Zinc_string* a, Zinc_string* b);
ZINC_API Zinc_string* Zinc_string_clone(Zinc_string* bf);

#ifdef __cplusplus
}
#endif

#endif
