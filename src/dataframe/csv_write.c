#include "data_frame.h"


Vector* CSVWrite(DataFrame* df)
{
    Vector* output = NULL;
    VectorCreate(&output, sizeof(char));

    size_t row_count = DataFrameRowCount(df);

    char comma = ',';
    Series* s = df->head;
    while (s) {
        if (s != df->head) {
            VectorAdd(output, &comma, 1);
        }
        VectorAdd(output, s->name.buffer, s->name.count);
        s = s->next;
    }
    char newline = '\n';
    VectorAdd(output, &newline, 1);

    size_t i = 0;
    while (i < row_count)
    {
        s = df->head;
        while (s) {
            if (s != df->head) {
                VectorAdd(output, &comma, 1);
            }
            VectorAdd(output,
                      VECTOR_VECTOR(&s->raw, i)->buffer,
                      VECTOR_VECTOR(&s->raw, i)->count);
            s = s->next;
        }

        VectorAdd(output, &newline, 1);
        i++;
    }

    VectorAddNull(output);
    return output;
}