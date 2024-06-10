#ifndef ZINC_CSV_PARSE_H
#define ZINC_CSV_PARSE_H

#include <stdbool.h>
#include "csv_lex.h"
#include "data_frame.h"

struct CSVParseData {
    struct error_list* el;
    struct CSVLexData* lex_data;
};

struct CSVParseOutput {
    Vector* input_text;
    struct error_list* el;
    DataFrame* df;
};

DataFrame* CSVParse(struct CSVParseData* parse_data);
void CSVParseDataInit(struct CSVParseData* parse_data);
void CSVParseDataCreate(struct CSVParseData** parse_data);

void CSVParseOutputInit(struct CSVParseOutput* parse_output);
void CSVParseOutputCreate(struct CSVParseOutput** parse_output);

#endif