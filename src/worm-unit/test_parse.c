#include "zinc/unit_test.h"
#include "worm/parse.h"
#include "worm/lex_data.h"
#include "worm/parse_data.h"
#include "worm/lex.h"
#include "zinc/input_unicode_string.h"
#include <string.h>
#include "test_parse_tools.h"

void test_parse_type_int32()
{   
    Zinc_test_name(__func__);

    Worm_parse_data* pd = NULL;

    test_parse_setup(&pd,
        "Scalar {}\n"
        ".Integer {.name=\"Int32\";.bit_count=32;.is_signed=true}\n"
    );

    Worm_ast* root = Worm_parse(pd);

    /* root */
    Zinc_expect_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Worm_ast_type_stmts, "type root");

    /* line 1 */
    Worm_ast* level0 = Worm_ast_get(root, 0);
    Zinc_assert_ptr(level0, "ptr level0");
    Zinc_expect_int_equal(level0->type, Worm_ast_type_level, "type level0");

    Worm_ast* number = Worm_ast_get(level0, 0);
    Zinc_assert_ptr(number, "ptr number");
    Zinc_expect_int_equal(number->type, Worm_ast_type_natural, "type number");
    Zinc_expect_unsigned_long_equal(number->number.natural, 0, "natrual number");

    Worm_ast* scalar = Worm_ast_get(level0, 1);
    Zinc_assert_ptr(scalar, "ptr scaler");
    Zinc_expect_int_equal(scalar->type, Worm_ast_type_object, "type scalar");
    Zinc_expect_string(&scalar->value, "Scalar", "value scalar");

    /* line 2 */

    test_parse_teardown(pd);
    Worm_ast_destroy(root);
    free(root);
}

void test_parse()
{
    test_parse_type_int32();
}
