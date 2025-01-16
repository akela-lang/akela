#ifndef ZINC_SERIES_H
#define ZINC_SERIES_H

#include "zinc/vector.h"
#include "field.h"

typedef struct Series {
    Zinc_vector name;
    enum FieldType type;
    Zinc_vector types;
    Zinc_vector raw;
    Zinc_vector value;
    struct Series* next;
    struct Series* prev;
} Series;

void SeriesInit(Series* s);
void SeriesCreate(Series** s);
void SeriesDestroy(Series* s);
void SeriesRefreshValues(Series* s);

#endif