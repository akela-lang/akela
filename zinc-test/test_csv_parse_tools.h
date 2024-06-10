#ifndef ZINC_TEST_CSV_PARSE_TOOLS_H
#define ZINC_TEST_CSV_PARSE_TOOLS_H

#include "zinc/csv_parse.h"

void CSVParseSetup(struct CSVParseOutput** parse_output, const char* text);
void CSVParseTeardown(struct CSVParseOutput* parse_output);

#endif