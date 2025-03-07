#include <assert.h>
#include "zinc/memory.h"
#include "series.h"
#include "field.h"

void SeriesInit(Series* s)
{
    Zinc_vector_init(&s->name, sizeof(char));
    s->type = FieldTypeNone;
    Zinc_vector_init(&s->types, sizeof(enum FieldType));
    Zinc_vector_init(&s->raw, sizeof(Zinc_vector*));
    Zinc_vector_init(&s->value, 0);
    s->next = NULL;
    s->prev = NULL;
}

void SeriesCreate(Series** s)
{
    Zinc_malloc_safe((void**)s, sizeof(Series));
    SeriesInit(*s);
}

void SeriesDestroy(Series* s)
{
    for (size_t i = 0; i < s->raw.count; i++) {
        Zinc_vector* v = ZINC_VECTOR_VECTOR(&s->raw, i);
        Zinc_vector_destroy(v);
        free(v);
    }
    Zinc_vector_destroy(&s->name);
    Zinc_vector_destroy(&s->types);
    Zinc_vector_destroy(&s->raw);
    Zinc_vector_destroy(&s->value);
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
            s->value.value_size = sizeof(uint8_t);
            break;
        case FieldTypeString:
            s->value.value_size = sizeof(Zinc_vector*);
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
                Zinc_vector* raw = *(Zinc_vector**)ZINC_VECTOR_PTR(&s->raw, i);
                double value = strtod(raw->buffer, NULL);
                Zinc_vector_add(&s->value, &value, 1);
                break;
            }
            case FieldTypeInt: {
                Zinc_vector* raw = *(Zinc_vector**)ZINC_VECTOR_PTR(&s->raw, i);
                long value = strtol(raw->buffer, NULL, 10);
                Zinc_vector_add(&s->value, &value, 1);
                break;
            }
            case FieldTypeIntU: {
                Zinc_vector* raw = *(Zinc_vector**)ZINC_VECTOR_PTR(&s->raw, i);
                unsigned long value = strtoul(raw->buffer, NULL, 10);
                Zinc_vector_add(&s->value, &value, 1);
                break;
            }
            case FieldTypeBool: {
                Zinc_vector* raw = *(Zinc_vector**)ZINC_VECTOR_PTR(&s->raw, i);
                uint8_t value;
                if (Zinc_vector_match_str(raw, "True")) {
                    value = 1;
                } else if (Zinc_vector_match_str(raw, "False")) {
                    value = 0;
                } else {
                    assert(false);
                }
                Zinc_vector_add(&s->value, &value, 1);
                break;
            }
            case FieldTypeString: {
                Zinc_vector* raw = *(Zinc_vector**)ZINC_VECTOR_PTR(&s->raw, i);
                Zinc_vector_add(&s->value, &raw, 1);
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