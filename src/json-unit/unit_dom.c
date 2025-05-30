#include "json/dom.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Json_unit_dom_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_ptr_equal(test, root->head, a, "head root");
    Zinc_expect_ptr_equal(test, a->prev, NULL, "prev a");
    Zinc_expect_ptr_equal(test, a->next, b, "next a");
    Zinc_expect_ptr_equal(test, b->prev, a, "prev b");
    Zinc_expect_ptr_equal(test, b->next, c, "next b");
    Zinc_expect_ptr_equal(test, c->prev, b, "prev c");
    Zinc_expect_ptr_equal(test, c->next, NULL, "next c");
    Zinc_expect_ptr_equal(test, root->tail, c, "tail root");

    Json_dom_destroy(root);
    free(root);
}

void Json_unit_dom(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Json_unit_dom_add);

        return;
    }

    Zinc_test_perform(test);
}