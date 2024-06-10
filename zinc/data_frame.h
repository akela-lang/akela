#ifndef SERIES_LIST_H
#define SERIES_LIST_H

#include "vector.h"
#include "series.h"

typedef struct {
    Series* head;
    Series* tail;
} DataFrame;

void DataFrameInit(DataFrame* sl);
void DataFrameCreate(DataFrame** sl);
void DataFrameAdd(DataFrame* df, Series* s);
void DataFrameDestroy(DataFrame* df);
Series* DataFrameSeriesByIndex(DataFrame* df, size_t index);
size_t DataFrameColumnCount(DataFrame* df);
size_t DataFrameRowCount(DataFrame* df);
void DataFramePrint(DataFrame* df);

#endif