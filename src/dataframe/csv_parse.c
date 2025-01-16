#include <assert.h>
#include "zinc/memory.h"
#include "csv_parse.h"
#include "csv_lex.h"
#include "field_lex.h"

bool CSVParseHeader(struct CSVParseData* parse_data, DataFrame* df);
bool CSVParseRow(struct CSVParseData* parse_data, DataFrame* df);

void CSVParseDataInit(struct CSVParseData* parse_data)
{
    parse_data->el = NULL;
    parse_data->lex_data = NULL;
}

void CSVParseDataCreate(struct CSVParseData** parse_data)
{
    Zinc_malloc_safe((void**)parse_data, sizeof(struct CSVParseData));
    CSVParseDataInit(*parse_data);
}

void CSVParseOutputInit(struct CSVParseOutput* parse_output)
{
    parse_output->input_text = NULL;
    parse_output->el = NULL;
    parse_output->df = NULL;
}

void CSVParseOutputCreate(struct CSVParseOutput** parse_output)
{
    Zinc_malloc_safe((void**)parse_output, sizeof(struct CSVParseOutput));
    CSVParseOutputInit(*parse_output);
}

/**
 * Parse a CSV file and add header and values to data frame.
 * @param parse_data parse data
 * @param df data frame
 */
DataFrame* CSVParse(struct CSVParseData* parse_data)
{
    bool done = false;
    DataFrame* df = NULL;
    DataFrameCreate(&df);

    done = CSVParseHeader(parse_data, df);
    while (!done) {
        done = CSVParseRow(parse_data, df);
    }

    Series* s = df->head;
    while (s) {
        SeriesRefreshValues(s);
        s = s->next;
    }

    return df;
}

/**
 * Add a series for each field in header row.
 * @param parse_data parse data
 * @param df dataframe add series to
 * @return true if done, otherwise false
 */
bool CSVParseHeader(struct CSVParseData* parse_data, DataFrame* df)
{
    while (true) {
        struct CSVToken* token = NULL;
        CSVLex(parse_data->lex_data, &token);
        if (token->type == CSVTokenTypeEOF) {
            CSVTokenDestroy(token);
            free(token);
            return true;
        } else if (token->type == CSVTokenTypeNewline) {
            CSVTokenDestroy(token);
            free(token);
            return false;
        } else if (token->type == CSVTokenTypeField) {
            Series* s = NULL;
            SeriesCreate(&s);
            s->type = FieldTypeEmpty;
            Zinc_vector_add(&s->name, token->value.buffer, token->value.count);
            DataFrameAdd(df, s);
            CSVTokenDestroy(token);
            free(token);
        } else {
            assert(false);
        }
    }
}

/**
 * Add row to data frame.
 * @param parse_data parse data
 * @param df data frame
 * @return true if done with input, otherwise false
 */
bool CSVParseRow(struct CSVParseData* parse_data, DataFrame* df)
{
    size_t i = 0;
    while (true) {
        struct CSVToken* token = NULL;
        CSVLex(parse_data->lex_data, &token);
        if (token->type == CSVTokenTypeEOF) {
            CSVTokenDestroy(token);
            free(token);
            return true;
        } else if (token->type == CSVTokenTypeNewline) {
            CSVTokenDestroy(token);
            free(token);
            return false;
        } else if (token->type == CSVTokenTypeField) {
            Series* s = DataFrameSeriesByIndex(df, i);
            if (!s) {
                Zinc_error_list_set(parse_data->el, &token->loc, "Row has more fields than header");
            } else {
                /* field type */
                enum FieldType field_type;
                FieldGetType(&token->value, &field_type);
                Zinc_vector_add(&s->types, &field_type, 1);
                s->type = FieldPromotion(field_type, s->type);

                /* field raw */
                Zinc_vector* raw = NULL;
                Zinc_vector_create(&raw, sizeof(char));
                Zinc_vector_add(raw, token->value.buffer, token->value.count);
                Zinc_vector_add_null(raw);
                Zinc_vector_add(&s->raw, &raw, 1);
            }
            i++;
            CSVTokenDestroy(token);
            free(token);
        } else {
            assert(false);
        }
    }
}