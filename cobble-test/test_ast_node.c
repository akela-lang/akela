#include "zinc/unit_test.h"
#include "zinc/memory.h"
#include "cobble/Ast_node.h"

void test_Ast_node_root()
{
    test_name(__func__);

    Ast_node* n = NULL;
    malloc_safe((void**)&n, sizeof(Ast_node));
    Ast_node_init(n);
    expect_int_equal(n->type, Ast_type_none, "type none");
    expect_int_equal(n->num, 0, "num 0");
    expect_ptr_equal(n->next, NULL, "next NULL");
    expect_ptr_equal(n->prev, NULL, "prev NULL");
    expect_ptr_equal(n->head, NULL, "head NULL");
    expect_ptr_equal(n->tail, NULL, "tail NULL");
    Ast_node_destroy(n);
}

void test_Ast_node_children()
{
    test_name(__func__);

    Ast_node* root = NULL;
    malloc_safe((void**)&root, sizeof(Ast_node));
    Ast_node_init(root);

    Ast_node* c0 = NULL;
    malloc_safe((void**)&c0, sizeof(Ast_node));
    Ast_node_init(c0);

    Ast_node* c1 = NULL;
    malloc_safe((void**)&c1, sizeof(Ast_node));
    Ast_node_init(c1);

    Ast_node* c2 = NULL;
    malloc_safe((void**)&c2, sizeof(Ast_node));
    Ast_node_init(c2);

    Ast_node_add(root, c0);
    Ast_node_add(root, c1);
    Ast_node_add(root, c2);

    expect_ptr_equal(root->head, c0, "head c0");
    expect_ptr_equal(root->tail, c2, "tail c2");

    Ast_node* n = root->head;

    assert_ptr(n, "0 n ptr");
    expect_ptr_equal(n, c0, "0 n c1");
    expect_ptr_equal(n->prev, NULL, "0 n->prev NULL");
    expect_ptr_equal(n->next, c1, "0 n->next c1");

    n = n->next;
    assert_ptr(n, "1 n ptr");
    expect_ptr_equal(n, c1, "1 n c1");
    expect_ptr_equal(n->prev, c0, "1 n->prev c0");
    expect_ptr_equal(n->next, c2, "1 n->next c2");

    n = n->next;
    assert_ptr(n, "2 n ptr");
    expect_ptr_equal(n, c2, "2 n c2");
    expect_ptr_equal(n->prev, c1, "2 n->prev c1");
    expect_ptr_equal(n->next, NULL, "2 n->next NULL");

    Ast_node_destroy(root);

}

void test_ast_node()
{
    test_Ast_node_root();
    test_Ast_node_children();
}