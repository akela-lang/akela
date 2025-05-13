#include "dataframe/csv_write.h"
#include "dataframe/csv_parse.h"
#include "unit_csv_parse_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void DfUnit_CSVWrite1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVParseOutput* parse_output = NULL;
    DfUnit_CSVParseSetup(&parse_output,
                  "Bool,Float,Int,IntU,String,Empty\n"
                  "True,1.2,-3,50,hello,\n"
                  "False,5.1,80,1,world,\n");

    Zinc_test_expect_no_errors(test, parse_output->el);
    Zinc_test_expect_size_t_equal(test, DataFrameColumnCount(parse_output->df), 6, "column count");
    Zinc_test_expect_size_t_equal(test, DataFrameRowCount(parse_output->df), 2, "row count");

    Zinc_vector* output_text = CSVWrite(parse_output->df);
    Zinc_test_expect_vector(test, parse_output->input_text, output_text, "text");

    DfUnit_CSVParseTeardown(parse_output);
    Zinc_vector_destroy(output_text);
    free(output_text);
}

void DfUnit_CSVWrite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, DfUnit_CSVWrite1);

        return;
    }

    Zinc_test_perform(test);
}