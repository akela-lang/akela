#ifndef ZINC_SERIES_H
#define ZINC_SERIES_H

#include "vector.h"
#include "field.h"

typedef struct Series {
    Vector name;
    enum FieldType type;
    Vector types;
    Vector raw;
    Vector value;
    struct Series* next;
    struct Series* prev;
} Series;

void SeriesInit(Series* s);
void SeriesCreate(Series** s);
void SeriesDestroy(Series* s);
void SeriesRefreshValues(Series* s);

#endif