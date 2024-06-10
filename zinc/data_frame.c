#include "data_frame.h"
#include "memory.h"
#include "buffer.h"
#include <stdbool.h>
#include "string.h"
#include <ctype.h>
#include "series.h"

void DataFrameInit(DataFrame* df)
{
    df->head = NULL;
    df->tail = NULL;
}

void DataFrameCreate(DataFrame** df)
{
    malloc_safe((void**)df, sizeof(DataFrame));
    DataFrameInit(*df);
}

/**
 * Append Vector node to end of list.
 * @param df Vector list
 * @param s Vector node
 */
void DataFrameAdd(DataFrame* df, Series* s)
{
    if (df->head == NULL) {
        df->head = s;
        df->tail = s;
    } else {
        s->prev = df->tail;
        df->tail->next = s;
        df->tail = s;
    }
}

void DataFrameDestroy(DataFrame* df)
{
    Series* s = df->head;
    while (s) {
        Series* temp = s;
        s = s->next;
        SeriesDestroy(temp);
        free(temp);
    }
}

Series* DataFrameSeriesByIndex(DataFrame* df, size_t index)
{
    size_t i = 0;
    Series* s = df->head;
    while (s) {
        if (i == index) {
            return s;
        }
        s = s->next;
        i++;
    }
    return NULL;
}

size_t DataFrameColumnCount(DataFrame* df)
{
    size_t count = 0;
    Series* s = df->head;
    while(s) {
        count++;
        s = s->next;
    }
    return count;
}

size_t DataFrameRowCount(DataFrame* df)
{
    Series* s = df->head;
    return s->value.count;
}

void DataFramePrint(DataFrame* df)
{
    int i = 0;
    Series* s = df->head;
    while (s) {
        if (i > 0) {
            printf(" ");
        }
        printf("%10s", VECTOR_STRING(&s->name));
        s = s->next;
        i++;
    }
    printf("\n");

    s = df->head;
    size_t count = s->value.count;
    size_t head = count;
    size_t tail = count;
    if (count > 20) {
        head = 10;
        tail = count - 10;
    }
    bool printed_filer = false;
    for (i = 0; i < count; i++) {
        if (i > head && i < tail) {
            if (!printed_filer) {
                printf("...\n");
                printed_filer = true;
            }
            continue;
        }
        s = df->head;
        int j = 0;
        while (s) {
            if (j > 0) {
                printf(" ");
            }
            if (s->type == FieldTypeString) {
                Vector *v = VECTOR_VECTOR(&s->raw, i);
                printf("%s", VECTOR_STRING(v));
            } else if (s->type == FieldTypeInt) {
                printf("%10ld", VECTOR_LONG(&s->value, i));
            } else if (s->type == FieldTypeInt || s->type == FieldTypeIntU) {
                printf("%10lu", VECTOR_LONG_U(&s->value, i));
            } else if (s->type == FieldTypeFloat) {
                printf("%10.4lf", VECTOR_DOUBLE(&s->value, i));
            }
            s = s->next;
            j++;
        }
        printf("\n");
    }
}