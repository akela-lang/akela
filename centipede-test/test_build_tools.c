#include "centipede/parse_data.h"
#include "test_parse_tools.h"

void test_build_teardown(Cent_parse_data* pd, Cent_parse_result* pr, Cent_value* root)
{
    bool need_teardown = Cent_value_destroy_setup();

    test_parse_teardown(pd, pr);
    Cent_value_free(root);

    if (need_teardown) {
        Cent_value_destroy_teardown();
    }
}