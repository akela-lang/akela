#include "zinc/unit_test.h"
#include "json/dom.h"


void test_dom_add()
{
    test_name(__func__);

    Json_dom* root = NULL;
    Json_dom_create(&root);
    Json_dom_set_type(root, Json_dom_type_array);

    Json_dom* a = NULL;
    Json_dom_create(&a);
    Json_dom_set_type(a, Json_dom_type_string);

    Json_dom* b = NULL;
    Json_dom_create(&b);
    Json_dom_set_type(b, Json_dom_type_array);

    Json_dom* c = NULL;
    Json_dom_create(&c);
    Json_dom_set_type(c, Json_dom_type_object);

    Json_dom_add_element(root, a);
    Json_dom_add_element(root, b);
    Json_dom_add_element(root, c);

    expect_ptr_equal(root->head, a, "head root");
    expect_ptr_equal(a->prev, NULL, "prev a");
    expect_ptr_equal(a->next, b, "next a");
    expect_ptr_equal(b->prev, a, "prev b");
    expect_ptr_equal(b->next, c, "next b");
    expect_ptr_equal(c->prev, b, "prev c");
    expect_ptr_equal(c->next, NULL, "next c");
    expect_ptr_equal(root->tail, c, "tail root");

    Json_dom_destroy(root);
}

void test_dom()
{
    test_dom_add();
}