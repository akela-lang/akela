#include "zinc/unit_test.h"
#include "test_data_frame.h"
#include "test_field_lex.h"
#include "test_csv_lex.h"
#include "test_csv_parse.h"
#include "test_csv_write.h"

int main()
{
    TestDataFrame();
    TestFieldLex();
    TestCSVLex();
    TestCSVParse();
    TestCSVWrite();
    print_results();
    return 0;
}