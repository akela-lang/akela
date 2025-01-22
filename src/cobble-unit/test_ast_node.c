#include "zinc/unit_test.h"
#include "zinc/memory.h"
#include "cobble/ast.h"

void test_Ast_node_root()
{
    Zinc_test_name(__func__);

    Cob_ast* n = NULL;
    Zinc_malloc_safe((void**)&n, sizeof(Cob_ast));
    Cob_ast_init(n);

    Zinc_expect_int_equal(n->type, Cob_ast_type_none, "type none");
    Zinc_expect_int_equal(n->num, 0, "num 0");
    Zinc_expect_ptr_equal(n->next, NULL, "next NULL");
    Zinc_expect_ptr_equal(n->prev, NULL, "prev NULL");
    Zinc_expect_ptr_equal(n->head, NULL, "head NULL");
    Zinc_expect_ptr_equal(n->tail, NULL, "tail NULL");

    Cob_ast_destroy(n);
    free(n);
}

void test_Ast_node_children()
{
    Zinc_test_name(__func__);

    Cob_ast* root = NULL;
    Zinc_malloc_safe((void**)&root, sizeof(Cob_ast));
    Cob_ast_init(root);

    Cob_ast* c0 = NULL;
    Zinc_malloc_safe((void**)&c0, sizeof(Cob_ast));
    Cob_ast_init(c0);

    Cob_ast* c1 = NULL;
    Zinc_malloc_safe((void**)&c1, sizeof(Cob_ast));
    Cob_ast_init(c1);

    Cob_ast* c2 = NULL;
    Zinc_malloc_safe((void**)&c2, sizeof(Cob_ast));
    Cob_ast_init(c2);

    Cob_ast_add(root, c0);
    Cob_ast_add(root, c1);
    Cob_ast_add(root, c2);

    Zinc_expect_ptr_equal(root->head, c0, "head c0");
    Zinc_expect_ptr_equal(root->tail, c2, "tail c2");

    Cob_ast* n = root->head;

    Zinc_assert_ptr(n, "0 n ptr");
    Zinc_expect_ptr_equal(n, c0, "0 n c1");
    Zinc_expect_ptr_equal(n->prev, NULL, "0 n->prev NULL");
    Zinc_expect_ptr_equal(n->next, c1, "0 n->next c1");

    n = n->next;
    Zinc_assert_ptr(n, "1 n ptr");
    Zinc_expect_ptr_equal(n, c1, "1 n c1");
    Zinc_expect_ptr_equal(n->prev, c0, "1 n->prev c0");
    Zinc_expect_ptr_equal(n->next, c2, "1 n->next c2");

    n = n->next;
    Zinc_assert_ptr(n, "2 n ptr");
    Zinc_expect_ptr_equal(n, c2, "2 n c2");
    Zinc_expect_ptr_equal(n->prev, c1, "2 n->prev c1");
    Zinc_expect_ptr_equal(n->next, NULL, "2 n->next NULL");

    Cob_ast_destroy(root);
    free(root);
}

void test_ast_node()
{
    test_Ast_node_root();
    test_Ast_node_children();
}