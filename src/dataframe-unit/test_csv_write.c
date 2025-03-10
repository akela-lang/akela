#include "zinc/unit_test.h"
#include "dataframe/csv_write.h"
#include "zinc/error_unit_test.h"
#include "dataframe/csv_parse.h"
#include "test_csv_parse_tools.h"

void TestCSVWrite1()
{
    Zinc_test_name(__func__);
    Zinc_test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output,
                  "Bool,Float,Int,IntU,String,Empty\n"
                  "True,1.2,-3,50,hello,\n"
                  "False,5.1,80,1,world,\n");

    Zinc_expect_no_errors(parse_output->el);
    Zinc_expect_size_t_equal(DataFrameColumnCount(parse_output->df), 6, "column count");
    Zinc_expect_size_t_equal(DataFrameRowCount(parse_output->df), 2, "row count");

    Zinc_vector* output_text = CSVWrite(parse_output->df);
    Zinc_expect_vector(parse_output->input_text, output_text, "text");

    CSVParseTeardown(parse_output);
    Zinc_vector_destroy(output_text);
    free(output_text);
}

void TestCSVWrite()
{
    TestCSVWrite1();
}