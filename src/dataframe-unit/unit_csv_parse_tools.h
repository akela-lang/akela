#ifndef ZINC_TEST_CSV_PARSE_TOOLS_H
#define ZINC_TEST_CSV_PARSE_TOOLS_H

#include "dataframe/csv_parse.h"

void DfUnit_CSVParseSetup(struct CSVParseOutput** parse_output, const char* text);
void DfUnit_CSVParseTeardown(struct CSVParseOutput* parse_output);

#endif