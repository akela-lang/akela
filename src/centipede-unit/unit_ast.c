#include "centipede/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CentUnit_ast_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_ast* n = NULL;
    Cent_ast_create(&n);

    Cent_ast* a = NULL;
    Cent_ast_create(&a);

    Cent_ast* b = NULL;
    Cent_ast_create(&b);

    Cent_ast* c = NULL;
    Cent_ast_create(&c);

    Cent_ast_add(n, a);
    Cent_ast_add(n, b);
    Cent_ast_add(n, c);

    Zinc_expect_ptr_equal(test, n->head, a, "head n");
    Zinc_expect_ptr_equal(test, a->prev, NULL, "prev a");
    Zinc_expect_ptr_equal(test, a->next, b, "next a");
    Zinc_expect_ptr_equal(test, b->prev, a, "prev b");
    Zinc_expect_ptr_equal(test, b->next, c, "next b");
    Zinc_expect_ptr_equal(test, c->prev, b, "prev c");
    Zinc_expect_ptr_equal(test, c->next, NULL, "next c");
    Zinc_expect_ptr_equal(test, n->tail, c, "tail n");

    Cent_ast_destroy(n);
    free(n);
}

void CentUnit_ast(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CentUnit_ast_add);

        return;
    }

    Zinc_test_perform(test);
}