#include "zinc/unit_test.h"
#include "worm/ast.h"

void test_ast_add()
{
    Zinc_test_name(__func__);
    
    Worm_ast* root = NULL;
    Worm_ast_create(&root);

    Worm_ast* n0 = NULL;
    Worm_ast_create(&n0);

    Worm_ast* n1 = NULL;
    Worm_ast_create(&n1);

    Worm_ast* n2 = NULL;
    Worm_ast_create(&n2);

    Worm_ast_add(root, n0);
    Worm_ast_add(root, n1);
    Worm_ast_add(root, n2);

    Zinc_expect_ptr_equal(root->head, n0, "head root");
    Zinc_expect_ptr_equal(n0->prev, NULL, "prev n0");
    Zinc_expect_ptr_equal(n0->next, n1, "next n0");
    Zinc_expect_ptr_equal(n1->prev, n0, "prev n1");
    Zinc_expect_ptr_equal(n1->next, n2, "next n1");
    Zinc_expect_ptr_equal(n2->prev, n1, "prev n2");
    Zinc_expect_ptr_equal(n2->next, NULL, "next n2");
    Zinc_expect_ptr_equal(root->tail, n2, "tail root");

    Worm_ast_destroy(root);
    free(root);
}

void test_ast()
{
    test_ast_add();
}
