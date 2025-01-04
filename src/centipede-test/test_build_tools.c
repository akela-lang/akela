#include "centipede/parse_data.h"
#include "test_parse_tools.h"

void test_build_teardown(Cent_parse_data* pd, Cent_parse_result* pr, Cent_value* root)
{
    bool need_teardown = Cent_value_destroy_setup();

    /* Free element tree before AST because variables in the AST symbol table might be */
    /* in element tree and we need to traverse the tree without elements being free */
    /* underneath us. */
    Cent_value_free(root);
    test_parse_teardown(pd, pr);

    if (need_teardown) {
        Cent_value_destroy_teardown();
    }
}