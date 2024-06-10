#ifndef ZINC_VECTOR_H
#define ZINC_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define SERIES_CHUNK_SIZE 64

typedef struct {
    void* buffer;
    size_t value_size;
    size_t count;
    size_t container_size;
} Vector;

#ifdef __cplusplus
extern "C" {
#endif
void VectorInit(Vector* v, size_t value_size);
void VectorCreate(Vector** v, size_t value_size);
void VectorExpand(Vector* v, size_t count);
void VectorAdd(Vector* v, const void* buffer, size_t count);
void VectorAddNull(Vector* v);
void VectorDestroy(Vector* v);
bool VectorMatch(Vector* a, Vector* b);
bool VectorMatchStr(Vector* a, const char b[]);
void VectorCopy(Vector* src, Vector* dest);
void VectorPrintDouble(Vector* v);

#ifdef __cplusplus
}
#endif

#define VECTOR_PTR(s, index) ((u_int8_t*)(s)->buffer + ((s)->value_size * (index)))
#define VECTOR_DOUBLE(s, index) (*(double*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_U_LONG(s, index) (*(unsigned long*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_INT(s, index) (*(int*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_INT_U(s, index) (*(unsigned int*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_LONG(s, index) (*(long*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_LONG_U(s, index) (*(unsigned long*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_BOOL(s, index) (*(u_int8_t*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_CHAR(s, index) (*(char*)((s)->buffer + ((s)->value_size * (index))))
#define VECTOR_STRING(s) ((char*)(s)->buffer)
#define VECTOR_VECTOR(s, index) (*(Vector**)((s)->buffer + ((s)->value_size * (index))))

#endif