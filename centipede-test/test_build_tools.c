#include "centipede/parse_data.h"
#include "test_parse_tools.h"

void test_build_teardown(Cent_parse_data* pd, Cent_parse_result* pr, Cent_value* root)
{
    test_parse_teardown(pd, pr);

    Cent_value_destroy_setup();
    Cent_value_free(root);
    Cent_value_destroy_teardown();
}