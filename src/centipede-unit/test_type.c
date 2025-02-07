#include "zinc/unit_test.h"
#include "centipede/type.h"

void test_type_param_list_add()
{
    Zinc_test_name(__func__);

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

    Zinc_expect_ptr_equal(list->head, tp0, "head list");
    Zinc_expect_ptr_equal(tp0->prev, NULL, "prev tp0");
    Zinc_expect_ptr_equal(tp0->next, tp1, "next tp0");
    Zinc_expect_ptr_equal(tp1->prev, tp0, "prev tp1");
    Zinc_expect_ptr_equal(tp1->next, tp2, "next tp1");
    Zinc_expect_ptr_equal(tp2->prev, tp1, "prev tp2");
    Zinc_expect_ptr_equal(tp2->next, NULL, "next tp2");
    Zinc_expect_ptr_equal(list->tail, tp2, "tail list");

    Cent_type_param_list_destroy(list);
    free(list);
}

void test_type_list_add()
{
    Zinc_test_name(__func__);

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

    Zinc_expect_ptr_equal(list->head, tn0, "head list");
    Zinc_expect_ptr_equal(tn0->prev, NULL, "prev tn0");
    Zinc_expect_ptr_equal(tn0->next, tn1, "next tn0");
    Zinc_expect_ptr_equal(tn1->prev, tn0, "prev tn1");
    Zinc_expect_ptr_equal(tn1->next, tn2, "next tn1");
    Zinc_expect_ptr_equal(tn2->prev, tn1, "prev tn2");
    Zinc_expect_ptr_equal(tn2->next, NULL, "next tn2");
    Zinc_expect_ptr_equal(list->tail, tn2, "tail list");

    Cent_type_list_destroy(list);
    free(list);
}

void test_type_field_list_add()
{
    Zinc_test_name(__func__);

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

    Zinc_expect_ptr_equal(list->head, tf0, "head list");
    Zinc_expect_ptr_equal(tf0->prev, NULL, "prev tf0");
    Zinc_expect_ptr_equal(tf0->next, tf1, "next tf0");
    Zinc_expect_ptr_equal(tf1->prev, tf0, "prev tf1");
    Zinc_expect_ptr_equal(tf1->next, tf2, "next tf1");
    Zinc_expect_ptr_equal(tf2->prev, tf1, "prev tf2");
    Zinc_expect_ptr_equal(tf2->next, NULL, "next tf2");
    Zinc_expect_ptr_equal(list->tail, tf2, "tail list");

    Cent_type_field_list_destroy(list);
    free(list);
}

void test_type()
{
    test_type_param_list_add();
    test_type_list_add();
    test_type_field_list_add();
}
