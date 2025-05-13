#include "unit_data_frame.h"
#include "unit_field_lex.h"
#include "unit_csv_lex.h"
#include "unit_csv_parse.h"
#include "unit_csv_write.h"
#include "zinc/test.h"

void DfUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        DfUnit_DataFrame(test);
        DfUnit_FieldLex(test);
        DfUnit_CSVLex(test);
        DfUnit_CSVParse(test);
        DfUnit_CSVWrite(test);

        return;
    }

    Zinc_test_perform(test);
}