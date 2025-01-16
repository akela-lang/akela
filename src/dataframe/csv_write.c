#include "data_frame.h"


Zinc_vector* CSVWrite(DataFrame* df)
{
    Zinc_vector* output = NULL;
    Zinc_vector_create(&output, sizeof(char));

    size_t row_count = DataFrameRowCount(df);

    char comma = ',';
    Series* s = df->head;
    while (s) {
        if (s != df->head) {
            Zinc_vector_add(output, &comma, 1);
        }
        Zinc_vector_add(output, s->name.buffer, s->name.count);
        s = s->next;
    }
    char newline = '\n';
    Zinc_vector_add(output, &newline, 1);

    size_t i = 0;
    while (i < row_count)
    {
        s = df->head;
        while (s) {
            if (s != df->head) {
                Zinc_vector_add(output, &comma, 1);
            }
            Zinc_vector_add(output,
                      ZINC_VECTOR_VECTOR(&s->raw, i)->buffer,
                      ZINC_VECTOR_VECTOR(&s->raw, i)->count);
            s = s->next;
        }

        Zinc_vector_add(output, &newline, 1);
        i++;
    }

    Zinc_vector_add_null(output);
    return output;
}