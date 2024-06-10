#include "zinc/unit_test.h"
#include "zinc/csv_write.h"
#include "zinc/error_unit_test.h"
#include "zinc/csv_parse.h"
#include "test_csv_parse_tools.h"

void TestCSVWrite1()
{
    test_name(__func__);
    test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output,
                  "Bool,Float,Int,IntU,String,Empty\n"
                  "True,1.2,-3,50,hello,\n"
                  "False,5.1,80,1,world,\n");

    expect_no_errors(parse_output->el);
    expect_size_t_equal(DataFrameColumnCount(parse_output->df), 6, "column count");
    expect_size_t_equal(DataFrameRowCount(parse_output->df), 2, "row count");

    Vector* output_text = CSVWrite(parse_output->df);
    expect_vector(parse_output->input_text, output_text, "text");

    CSVParseTeardown(parse_output);
    VectorDestroy(output_text);
    free(output_text);
}

void TestCSVWrite()
{
    TestCSVWrite1();
}