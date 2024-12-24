#include "zinc/unit_test.h"
#include "centipede/ast.h"

void test_ast_add()
{
    test_name(__func__);

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

    expect_ptr_equal(n->head, a, "head n");
    expect_ptr_equal(a->prev, NULL, "prev a");
    expect_ptr_equal(a->next, b, "next a");
    expect_ptr_equal(b->prev, a, "prev b");
    expect_ptr_equal(b->next, c, "next b");
    expect_ptr_equal(c->prev, b, "prev c");
    expect_ptr_equal(c->next, NULL, "next c");
    expect_ptr_equal(n->tail, c, "tail n");

    Cent_ast_destroy(n);
    free(n);
}

void test_ast()
{
    test_ast_add();
}