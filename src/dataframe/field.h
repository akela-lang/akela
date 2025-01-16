#ifndef ZINC_FIELD_H
#define ZINC_FIELD_H

enum FieldType {
    FieldTypeNone,
    FieldTypeEmpty,
    FieldTypeIntU,
    FieldTypeInt,
    FieldTypeFloat,
    FieldTypeBool,
    FieldTypeString,
};

#include "zinc/vector.h"

struct Field {
    enum FieldType type;
    Zinc_vector raw;
};

void FieldInit(struct Field* field);
void FieldDestroy(struct Field* field);
enum FieldType FieldPromotion(enum FieldType f1, enum FieldType f2);

#endif