#include "field.h"
#include <assert.h>

void FieldInit(struct Field* field)
{
    field->type = FieldTypeNone;
    VectorInit(&field->raw, sizeof(char));
}

void FieldDestroy(struct Field* field)
{
    VectorDestroy(&field->raw);
}

bool FieldIsNumber(enum FieldType type)
{
    if (type == FieldTypeIntU)
        return true;
    if (type == FieldTypeInt)
        return true;
    if (type == FieldTypeFloat)
        return true;
    return false;
}

enum FieldType FieldPromotion(enum FieldType f1, enum FieldType f2)
{
    assert(f1 != FieldTypeNone);
    assert(f2 != FieldTypeNone);

    if (f1 == f2)
        return f1;

    if (f1 == FieldTypeEmpty || f1 == FieldTypeNone)
        return f2;
    if (f2 == FieldTypeEmpty || f2 == FieldTypeNone)
        return f1;

    if (FieldIsNumber(f1) && FieldIsNumber(f2)) {
        if (f1 > f2)
            return f1;
        else
            return f2;
    }

    return FieldTypeString;
}