#include "zinc/memory.h"
#include "cobble/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CobUnit_Ast_node_root(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_ast* n = NULL;
    Zinc_malloc_safe((void**)&n, sizeof(Cob_ast));
    Cob_ast_init(n);

    Zinc_expect_int_equal(test, n->type, Cob_ast_type_none, "type none");
    Zinc_expect_int_equal(test, n->num, 0, "num 0");
    Zinc_expect_ptr_equal(test, n->next, NULL, "next NULL");
    Zinc_expect_ptr_equal(test, n->prev, NULL, "prev NULL");
    Zinc_expect_ptr_equal(test, n->head, NULL, "head NULL");
    Zinc_expect_ptr_equal(test, n->tail, NULL, "tail NULL");

    Cob_ast_destroy(n);
    free(n);
}

void CobUnit_Ast_node_children(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_ptr_equal(test, root->head, c0, "head c0");
    Zinc_expect_ptr_equal(test, root->tail, c2, "tail c2");

    Cob_ast* n = root->head;

    Zinc_assert_ptr(test, n, "0 n ptr");
    Zinc_expect_ptr_equal(test, n, c0, "0 n c1");
    Zinc_expect_ptr_equal(test, n->prev, NULL, "0 n->prev NULL");
    Zinc_expect_ptr_equal(test, n->next, c1, "0 n->next c1");

    n = n->next;
    Zinc_assert_ptr(test, n, "1 n ptr");
    Zinc_expect_ptr_equal(test, n, c1, "1 n c1");
    Zinc_expect_ptr_equal(test, n->prev, c0, "1 n->prev c0");
    Zinc_expect_ptr_equal(test, n->next, c2, "1 n->next c2");

    n = n->next;
    Zinc_assert_ptr(test, n, "2 n ptr");
    Zinc_expect_ptr_equal(test, n, c2, "2 n c2");
    Zinc_expect_ptr_equal(test, n->prev, c1, "2 n->prev c1");
    Zinc_expect_ptr_equal(test, n->next, NULL, "2 n->next NULL");

    Cob_ast_destroy(root);
    free(root);
}

void CobUnit_ast_node(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CobUnit_Ast_node_root);
        Zinc_test_register(test, CobUnit_Ast_node_children);

        return;
    }

    Zinc_test_perform(test);
}