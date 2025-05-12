#include "test_data_frame.h"
#include "test_field_lex.h"
#include "unit_csv_lex.h"
#include "test_csv_parse.h"
#include "test_csv_write.h"
#include "zinc/test.h"

void DfUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        // TestDataFrame();
        // TestFieldLex();
        DfUnit_CSVLex(test);
        // TestCSVParse();
        // TestCSVWrite();
        // Zinc_print_results();

        return;
    }

    Zinc_test_perform(test);
}