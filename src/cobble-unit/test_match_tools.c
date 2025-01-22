#include "zinc/unit_test.h"
#include "cobble/match_tools.h"

void test_match_tools_match_task_stack()
{
    Zinc_test_name(__func__);
    Cob_stack* mts = NULL;
    Cob_stack_create(&mts, NULL);

    Cob_task* mt0 = NULL;
    Cob_task_create(&mt0, NULL);

    Cob_task* mt1 = NULL;
    Cob_task_create(&mt1, NULL);

    Cob_stack_push(mts, mt0);
    Cob_stack_push(mts, mt1);

    Zinc_expect_ptr_equal(mts->top, mt1, "top mts");
    Zinc_expect_ptr_equal(mt1->prev, NULL, "prev mt1");
    Zinc_expect_ptr_equal(mt1->next, mt0, "next mt1");
    Zinc_expect_ptr_equal(mt0->prev, mt1, "prev mt0");
    Zinc_expect_ptr_equal(mt0->next, NULL, "next mt0");
    Zinc_expect_ptr_equal(mts->bottom, mt0, "bottom mts");

    Cob_stack_destroy(mts);

    Cob_stack_list* sl = NULL;
    Cob_stack_list_create(&sl);

    Cob_stack_node* sn0 = NULL;
    Cob_stack_node_create(&sn0);

    Cob_stack_node* sn1 = NULL;
    Cob_stack_node_create(&sn1);

    Cob_stack_list_add(sl, sn0);
    Cob_stack_list_add(sl, sn1);

    Zinc_expect_ptr_equal(sl->head, sn0, "head sn0");
    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, sn1, "next sn0");
    Zinc_expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    Zinc_expect_ptr_equal(sn1->next, NULL, "next sn1");
    Zinc_expect_ptr_equal(sl->tail, sn1, "tail sn1");

    Cob_stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_middle()
{
    Zinc_test_name(__func__);

    Cob_stack_list* sl = NULL;
    Cob_stack_list_create(&sl);

    Cob_stack_node* sn0 = NULL;
    Cob_stack_node_create(&sn0);

    Cob_stack_node* sn1 = NULL;
    Cob_stack_node_create(&sn1);

    Cob_stack_node* sn2 = NULL;
    Cob_stack_node_create(&sn2);

    Cob_stack_list_add(sl, sn0);
    Cob_stack_list_add(sl, sn1);
    Cob_stack_list_add(sl, sn2);

    Zinc_expect_ptr_equal(sl->head, sn0, "head sl");
    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, sn1, "next sn0");
    Zinc_expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    Zinc_expect_ptr_equal(sn1->next, sn2, "next sn1");
    Zinc_expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    Zinc_expect_ptr_equal(sn2->next, NULL, "next sn2");
    Zinc_expect_ptr_equal(sl->tail, sn2, "tail sl");

    Cob_stack_list_remove(sl, sn1);

    Zinc_expect_ptr_equal(sl->head, sn0, "head sl");
    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, sn2, "next sn0");
    Zinc_expect_ptr_equal(sn2->prev, sn0, "prev sn2");
    Zinc_expect_ptr_equal(sn2->next, NULL, "next sn2");
    Zinc_expect_ptr_equal(sl->tail, sn2, "tail sl");

    Zinc_expect_ptr_equal(sn1->prev, NULL, "prev sn1");
    Zinc_expect_ptr_equal(sn1->next, NULL, "next sn1");

    Cob_stack_node_destroy(sn1);
    Cob_stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_head()
{
    Zinc_test_name(__func__);

    Cob_stack_list* sl = NULL;
    Cob_stack_list_create(&sl);

    Cob_stack_node* sn0 = NULL;
    Cob_stack_node_create(&sn0);

    Cob_stack_node* sn1 = NULL;
    Cob_stack_node_create(&sn1);

    Cob_stack_node* sn2 = NULL;
    Cob_stack_node_create(&sn2);

    Cob_stack_list_add(sl, sn0);
    Cob_stack_list_add(sl, sn1);
    Cob_stack_list_add(sl, sn2);

    Zinc_expect_ptr_equal(sl->head, sn0, "head sl");
    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, sn1, "next sn0");
    Zinc_expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    Zinc_expect_ptr_equal(sn1->next, sn2, "next sn1");
    Zinc_expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    Zinc_expect_ptr_equal(sn2->next, NULL, "next sn2");
    Zinc_expect_ptr_equal(sl->tail, sn2, "tail sl");

    Cob_stack_list_remove(sl, sn0);

    Zinc_expect_ptr_equal(sl->head, sn1, "head sl");
    Zinc_expect_ptr_equal(sn1->prev, NULL, "prev sn1");
    Zinc_expect_ptr_equal(sn1->next, sn2, "next sn1");
    Zinc_expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    Zinc_expect_ptr_equal(sn2->next, NULL, "next sn2");
    Zinc_expect_ptr_equal(sl->tail, sn2, "tail sl");

    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, NULL, "next sn0");

    Cob_stack_node_destroy(sn0);
    Cob_stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_tail()
{
    Zinc_test_name(__func__);

    Cob_stack_list* sl = NULL;
    Cob_stack_list_create(&sl);

    Cob_stack_node* sn0 = NULL;
    Cob_stack_node_create(&sn0);

    Cob_stack_node* sn1 = NULL;
    Cob_stack_node_create(&sn1);

    Cob_stack_node* sn2 = NULL;
    Cob_stack_node_create(&sn2);

    Cob_stack_list_add(sl, sn0);
    Cob_stack_list_add(sl, sn1);
    Cob_stack_list_add(sl, sn2);

    Zinc_expect_ptr_equal(sl->head, sn0, "head sl");
    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, sn1, "next sn0");
    Zinc_expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    Zinc_expect_ptr_equal(sn1->next, sn2, "next sn1");
    Zinc_expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    Zinc_expect_ptr_equal(sn2->next, NULL, "next sn2");
    Zinc_expect_ptr_equal(sl->tail, sn2, "tail sl");

    Cob_stack_list_remove(sl, sn2);

    Zinc_expect_ptr_equal(sl->head, sn0, "head sl");
    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, sn1, "next sn0");
    Zinc_expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    Zinc_expect_ptr_equal(sn1->next, NULL, "next sn1");
    Zinc_expect_ptr_equal(sl->tail, sn1, "tail sl");

    Zinc_expect_ptr_equal(sn2->prev, NULL, "prev sn2");
    Zinc_expect_ptr_equal(sn2->next, NULL, "next sn2");

    Cob_stack_node_destroy(sn2);
    Cob_stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_single()
{
    Zinc_test_name(__func__);

    Cob_stack_list* sl = NULL;
    Cob_stack_list_create(&sl);

    Cob_stack_node* sn0 = NULL;
    Cob_stack_node_create(&sn0);

    Cob_stack_list_add(sl, sn0);

    Zinc_expect_ptr_equal(sl->head, sn0, "head sl");
    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, NULL, "next sn0");
    Zinc_expect_ptr_equal(sl->tail, sn0, "tail sl");

    Cob_stack_list_remove(sl, sn0);

    Zinc_expect_ptr_equal(sl->head, NULL, "head sl");
    Zinc_expect_ptr_equal(sl->tail, NULL, "tail sl");

    Zinc_expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    Zinc_expect_ptr_equal(sn0->next, NULL, "next sn0");

    Cob_stack_node_destroy(sn0);
    Cob_stack_list_destroy(sl);
}

void test_match_tools_clone()
{
    Zinc_test_name(__func__);

    Cob_stack* mts = NULL;
    Cob_stack_create(&mts, NULL);

    Cob_task* mt0 = NULL;
    Cob_task_create(&mt0, NULL);

    Cob_task* mt1 = NULL;
    Cob_task_create(&mt1, NULL);

    Cob_task* mt2 = NULL;
    Cob_task_create(&mt2, mt0);

    Cob_task* mt3 = NULL;
    Cob_task_create(&mt3, mt1);

    Cob_stack_push(mts, mt0);
    Cob_stack_push(mts, mt1);
    Cob_stack_push(mts, mt2);
    Cob_stack_push(mts, mt3);

    struct Zinc_string* bf = NULL;
    Zinc_string_create(&bf);

    struct Zinc_hash_table ht;
    Zinc_hash_map_string_init(&ht, 32);

    Cob_stack* new_mts = Cob_stack_clone(mts, &ht, NULL);
    Cob_task* new_mt3 = new_mts->top;
    Cob_task* new_mt2 = new_mt3->next;
    Cob_task* new_mt1 = new_mt2->next;
    Cob_task* new_mt0 = new_mt1->next;

    Zinc_expect_ptr_equal(new_mts->top, new_mt3, "top new_mts");
    Zinc_expect_ptr_equal(new_mt3->prev, NULL, "prev new_mt3");
    Zinc_expect_ptr_equal(new_mt3->next, new_mt2, "next new_mt3");
    Zinc_expect_ptr_equal(new_mt2->prev, new_mt3, "prev new_mt2");
    Zinc_expect_ptr_equal(new_mt2->next, new_mt1, "next new_mt2");
    Zinc_expect_ptr_equal(new_mt1->prev, new_mt2, "prev new_mt1");
    Zinc_expect_ptr_equal(new_mt1->next, new_mt0, "next new_mt1");
    Zinc_expect_ptr_equal(new_mt0->prev, new_mt1, "prev new_mt0");
    Zinc_expect_ptr_equal(new_mt0->next, NULL, "next new_mt0");
    Zinc_expect_ptr_equal(mts->bottom, mt0, "bottom mts");

    Zinc_expect_ptr_equal(new_mt2->parent, new_mt0, "parent new_mt2");
    Zinc_expect_ptr_equal(new_mt3->parent, new_mt1, "parent new_mt3");

    Cob_stack_destroy(mts);
    Cob_stack_destroy(new_mts);
    Zinc_string_destroy(bf);
    free(bf);
    Zinc_hash_map_string_destroy(&ht);
}

void test_match_tools_stack_node_clone()
{
    Zinc_test_name(__func__);

    struct Zinc_string* bf = NULL;
    Zinc_string_create(&bf);
    Zinc_string_add_str(bf, "abc");

    Cob_stack_node* sn0 = NULL;
    Cob_stack_node_create(&sn0);
    Zinc_hash_map_size_t_add(&sn0->groups, 0, bf);

    Cob_stack_node* sn1 = Cob_stack_node_clone(sn0);

    struct Zinc_string* bf2 = Zinc_hash_map_size_t_get(&sn0->groups, 0);
    Zinc_expect_ptr(bf2, "ptr bf2");
    Zinc_expect_string(bf2, "abc", "str bf2");

    Cob_stack_node_destroy(sn0);
    Cob_stack_node_destroy(sn1);
}

void test_match_tools_stack_priority()
{
    Zinc_test_name(__func__);

    Cob_stack_list* sl = NULL;
    Cob_stack_list_create(&sl);

    Cob_stack_node* sn30 = NULL;
    Cob_stack_node_create(&sn30);
    sn30->priority = 30;

    Cob_stack_node* sn20 = NULL;
    Cob_stack_node_create(&sn20);
    sn20->priority = 20;

    Cob_stack_node* sn10 = NULL;
    Cob_stack_node_create(&sn10);
    sn10->priority = 10;

    Cob_stack_node* sn40 = NULL;
    Cob_stack_node_create(&sn40);
    sn40->priority = 40;

    Cob_stack_list_add(sl, sn30);
    Cob_stack_list_add(sl, sn20);
    Cob_stack_list_add(sl, sn10);
    Cob_stack_list_add(sl, sn40);

    Cob_stack_node* x = NULL;

    x = sl->tail;
    Zinc_expect_size_t_equal(x->priority, 40, "priority 40");
    Cob_stack_list_remove(sl, x);
    Cob_stack_node_destroy(x);

    x = sl->tail;
    Zinc_expect_size_t_equal(x->priority, 30, "priority 40");
    Cob_stack_list_remove(sl, x);
    Cob_stack_node_destroy(x);

    x = sl->tail;
    Zinc_expect_size_t_equal(x->priority, 20, "priority 40");
    Cob_stack_list_remove(sl, x);
    Cob_stack_node_destroy(x);

    x = sl->tail;
    Zinc_expect_size_t_equal(x->priority, 10, "priority 40");
    Cob_stack_list_remove(sl, x);
    Cob_stack_node_destroy(x);

    Cob_stack_list_destroy(sl);
}

void test_match_tools()
{
    test_match_tools_match_task_stack();
    test_match_tools_stack_list_remove_middle();
    test_match_tools_stack_list_remove_head();
    test_match_tools_stack_list_remove_tail();
    test_match_tools_stack_list_remove_single();
    test_match_tools_clone();
    test_match_tools_stack_node_clone();
    test_match_tools_stack_priority();
}