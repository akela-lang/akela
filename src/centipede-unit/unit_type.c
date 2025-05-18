#include <centipede/element.h>
#include "centipede/type.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CentUnit_type_param_list_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_type_param_list* list = NULL;
    Cent_type_param_list_create(&list);

    Cent_type_param* tp0 = NULL;
    Cent_type_param_create(&tp0);

    Cent_type_param* tp1 = NULL;
    Cent_type_param_create(&tp1);

    Cent_type_param* tp2 = NULL;
    Cent_type_param_create(&tp2);

    Cent_type_param_list_add(list, tp0);
    Cent_type_param_list_add(list, tp1);
    Cent_type_param_list_add(list, tp2);

    Zinc_test_expect_ptr_equal(test, list->head, tp0, "head list");
    Zinc_test_expect_ptr_equal(test, tp0->prev, NULL, "prev tp0");
    Zinc_test_expect_ptr_equal(test, tp0->next, tp1, "next tp0");
    Zinc_test_expect_ptr_equal(test, tp1->prev, tp0, "prev tp1");
    Zinc_test_expect_ptr_equal(test, tp1->next, tp2, "next tp1");
    Zinc_test_expect_ptr_equal(test, tp2->prev, tp1, "prev tp2");
    Zinc_test_expect_ptr_equal(test, tp2->next, NULL, "next tp2");
    Zinc_test_expect_ptr_equal(test, list->tail, tp2, "tail list");

    Cent_type_param_list_destroy(list);
    free(list);
}

void CentUnit_type_list_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_type_list* list = NULL;
    Cent_type_list_create(&list);

    Cent_type_node* tn0 = NULL;
    Cent_type_node_create(&tn0);

    Cent_type_node* tn1 = NULL;
    Cent_type_node_create(&tn1);

    Cent_type_node* tn2 = NULL;
    Cent_type_node_create(&tn2);

    Cent_type_list_add(list, tn0);
    Cent_type_list_add(list, tn1);
    Cent_type_list_add(list, tn2);

    Zinc_test_expect_ptr_equal(test, list->head, tn0, "head list");
    Zinc_test_expect_ptr_equal(test, tn0->prev, NULL, "prev tn0");
    Zinc_test_expect_ptr_equal(test, tn0->next, tn1, "next tn0");
    Zinc_test_expect_ptr_equal(test, tn1->prev, tn0, "prev tn1");
    Zinc_test_expect_ptr_equal(test, tn1->next, tn2, "next tn1");
    Zinc_test_expect_ptr_equal(test, tn2->prev, tn1, "prev tn2");
    Zinc_test_expect_ptr_equal(test, tn2->next, NULL, "next tn2");
    Zinc_test_expect_ptr_equal(test, list->tail, tn2, "tail list");

    Cent_type_list_destroy(list);
    free(list);
}

void CentUnit_type_field_list_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_type_field_list* list = NULL;
    Cent_type_field_list_create(&list);

    Cent_type_field* tf0 = NULL;
    Cent_type_field_create(&tf0);

    Cent_type_field* tf1 = NULL;
    Cent_type_field_create(&tf1);

    Cent_type_field* tf2 = NULL;
    Cent_type_field_create(&tf2);

    Cent_type_field_list_add(list, tf0);
    Cent_type_field_list_add(list, tf1);
    Cent_type_field_list_add(list, tf2);

    Zinc_test_expect_ptr_equal(test, list->head, tf0, "head list");
    Zinc_test_expect_ptr_equal(test, tf0->prev, NULL, "prev tf0");
    Zinc_test_expect_ptr_equal(test, tf0->next, tf1, "next tf0");
    Zinc_test_expect_ptr_equal(test, tf1->prev, tf0, "prev tf1");
    Zinc_test_expect_ptr_equal(test, tf1->next, tf2, "next tf1");
    Zinc_test_expect_ptr_equal(test, tf2->prev, tf1, "prev tf2");
    Zinc_test_expect_ptr_equal(test, tf2->next, NULL, "next tf2");
    Zinc_test_expect_ptr_equal(test, list->tail, tf2, "tail list");

    Cent_type_field_list_destroy(list);
    free(list);
}

void CentUnit_variant_list_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_variant_list list;
    Cent_variant_list_init(&list);

    Cent_variant_type* vt0 = NULL;
    Cent_variant_type_create(&vt0);

    Cent_variant_type* vt1 = NULL;
    Cent_variant_type_create(&vt1);

    Cent_variant_type* vt2 = NULL;
    Cent_variant_type_create(&vt2);

    Cent_variant_list_add(&list, vt0);
    Cent_variant_list_add(&list, vt1);
    Cent_variant_list_add(&list, vt2);

    Zinc_test_expect_ptr_equal(test, list.head, vt0, "head list");
    Zinc_test_expect_ptr_equal(test, vt0->prev, NULL, "prev vt0");
    Zinc_test_expect_ptr_equal(test, vt0->next, vt1, "next vt0");
    Zinc_test_expect_ptr_equal(test, vt1->prev, vt0, "prev vt1");
    Zinc_test_expect_ptr_equal(test, vt1->next, vt2, "next vt1");
    Zinc_test_expect_ptr_equal(test, vt2->prev, vt1, "prev vt2");
    Zinc_test_expect_ptr_equal(test, vt2->next, NULL, "next vt2");
    Zinc_test_expect_ptr_equal(test, list.tail, vt2, "tail list");

    Cent_variant_list_destroy(&list);
}

void CentUnit_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CentUnit_type_param_list_add);
        Zinc_test_register(test, CentUnit_type_list_add);
        Zinc_test_register(test, CentUnit_type_field_list_add);
        Zinc_test_register(test, CentUnit_variant_list_add);

        return;
    }

    Zinc_test_perform(test);
}
