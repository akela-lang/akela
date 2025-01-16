#include "dataframe/csv_parse.h"
#include <string.h>
#include "zinc/input_char_string.h"

void CSVParseSetup(struct CSVParseOutput** parse_output, const char* text)
{
    Zinc_vector* input = NULL;
    size_t len = strlen(text);
    Zinc_vector_create(&input, sizeof(char));
    Zinc_vector_add(input, (char*)(text), len);
    Zinc_vector_add_null(input);

    Zinc_input_char_string* input_data;
    Zinc_input_char_string_create(&input_data, input);

    struct CSVLexData* lex_data = NULL;
    CSVLexDataCreate(&lex_data);
    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);
    lex_data->el = el;
    lex_data->input_data = input_data;
    lex_data->input_vtable = input_data->input_vtable;

    struct CSVParseData* parse_data = NULL;
    CSVParseDataCreate(&parse_data);
    parse_data->el = el;
    parse_data->lex_data = lex_data;

    DataFrame* df = NULL;
    df = CSVParse(parse_data);

    CSVParseOutputCreate(parse_output);
    (*parse_output)->input_text = input;
    (*parse_output)->el = el;
    (*parse_output)->df = df;

    free(input_data);
    free(lex_data);
    free(parse_data);
}

void CSVParseTeardown(struct CSVParseOutput* parse_output)
{
    Zinc_vector_destroy(parse_output->input_text);
    free(parse_output->input_text);
    Zinc_error_list_destroy(parse_output->el);
    free(parse_output->el);
    DataFrameDestroy(parse_output->df);
    free(parse_output->df);
    free(parse_output);
}
