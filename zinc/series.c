#include "series.h"
#include "memory.h"
#include "field.h"
#include <assert.h>

void SeriesInit(Series* s)
{
    VectorInit(&s->name, sizeof(char));
    s->type = FieldTypeNone;
    VectorInit(&s->types, sizeof(enum FieldType));
    VectorInit(&s->raw, sizeof(Vector*));
    VectorInit(&s->value, 0);
    s->next = NULL;
    s->prev = NULL;
}

void SeriesCreate(Series** s)
{
    malloc_safe((void**)s, sizeof(Series));
    SeriesInit(*s);
}

void SeriesDestroy(Series* s)
{
    for (size_t i = 0; i < s->raw.count; i++) {
        Vector* v = VECTOR_VECTOR(&s->raw, i);
        VectorDestroy(v);
        free(v);
    }
    VectorDestroy(&s->name);
    VectorDestroy(&s->types);
    VectorDestroy(&s->raw);
    VectorDestroy(&s->value);
}

void SeriesRefreshValues(Series* s)
{
    switch (s->type) {
        case FieldTypeFloat:
            s->value.value_size = sizeof(double);
            break;
        case FieldTypeInt:
            s->value.value_size = sizeof(long);
            break;
        case FieldTypeIntU:
            s->value.value_size = sizeof(unsigned long);
            break;
        case FieldTypeBool:
            s->value.value_size = sizeof(u_int8_t);
            break;
        case FieldTypeString:
            s->value.value_size = sizeof(Vector*);
            break;
        case FieldTypeEmpty:
            s->value.value_size = 0;
            break;
        case FieldTypeNone:
            assert(false);
        default:
            assert(false);
    }

    s->value.count = 0;

    for (size_t i = 0; i < s->raw.count; i++) {
        switch (s->type) {
            case FieldTypeFloat: {
                Vector* raw = *(Vector**)VECTOR_PTR(&s->raw, i);
                double value = strtod(raw->buffer, NULL);
                VectorAdd(&s->value, &value, 1);
                break;
            }
            case FieldTypeInt: {
                Vector* raw = *(Vector**)VECTOR_PTR(&s->raw, i);
                long value = strtol(raw->buffer, NULL, 10);
                VectorAdd(&s->value, &value, 1);
                break;
            }
            case FieldTypeIntU: {
                Vector* raw = *(Vector**)VECTOR_PTR(&s->raw, i);
                unsigned long value = strtoul(raw->buffer, NULL, 10);
                VectorAdd(&s->value, &value, 1);
                break;
            }
            case FieldTypeBool: {
                Vector* raw = *(Vector**)VECTOR_PTR(&s->raw, i);
                u_int8_t value;
                if (VectorMatchStr(raw, "True")) {
                    value = 1;
                } else if (VectorMatchStr(raw, "False")) {
                    value = 0;
                } else {
                    assert(false);
                }
                VectorAdd(&s->value, &value, 1);
                break;
            }
            case FieldTypeString: {
                Vector* raw = *(Vector**)VECTOR_PTR(&s->raw, i);
                VectorAdd(&s->value, &raw, 1);
            }
            case FieldTypeEmpty:
                break;
            case FieldTypeNone: {
                assert(false);
            }
            default: {
                assert(false);
            }
        }
    }
}