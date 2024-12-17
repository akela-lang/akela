#include "zinc/unit_test.h"
#include "cobble/match_tools.h"

void test_match_tools_match_task_stack()
{
    test_name(__func__);
    Match_task_stack* mts = NULL;
    Match_task_stack_create(&mts, NULL);

    Match_task* mt0 = NULL;
    Match_task_create(&mt0, NULL);

    Match_task* mt1 = NULL;
    Match_task_create(&mt1, NULL);

    Match_task_stack_push(mts, mt0);
    Match_task_stack_push(mts, mt1);

    expect_ptr_equal(mts->top, mt1, "top mts");
    expect_ptr_equal(mt1->prev, NULL, "prev mt1");
    expect_ptr_equal(mt1->next, mt0, "next mt1");
    expect_ptr_equal(mt0->prev, mt1, "prev mt0");
    expect_ptr_equal(mt0->next, NULL, "next mt0");
    expect_ptr_equal(mts->bottom, mt0, "bottom mts");

    Match_task_stack_destroy(mts);

    Stack_list* sl = NULL;
    Stack_list_create(&sl);

    Stack_node* sn0 = NULL;
    Stack_node_create(&sn0);

    Stack_node* sn1 = NULL;
    Stack_node_create(&sn1);

    Stack_list_add(sl, sn0);
    Stack_list_add(sl, sn1);

    expect_ptr_equal(sl->head, sn0, "head sn0");
    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, sn1, "next sn0");
    expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    expect_ptr_equal(sn1->next, NULL, "next sn1");
    expect_ptr_equal(sl->tail, sn1, "tail sn1");

    Stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_middle()
{
    test_name(__func__);

    Stack_list* sl = NULL;
    Stack_list_create(&sl);

    Stack_node* sn0 = NULL;
    Stack_node_create(&sn0);

    Stack_node* sn1 = NULL;
    Stack_node_create(&sn1);

    Stack_node* sn2 = NULL;
    Stack_node_create(&sn2);

    Stack_list_add(sl, sn0);
    Stack_list_add(sl, sn1);
    Stack_list_add(sl, sn2);

    expect_ptr_equal(sl->head, sn0, "head sl");
    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, sn1, "next sn0");
    expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    expect_ptr_equal(sn1->next, sn2, "next sn1");
    expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    expect_ptr_equal(sn2->next, NULL, "next sn2");
    expect_ptr_equal(sl->tail, sn2, "tail sl");

    Stack_list_remove(sl, sn1);

    expect_ptr_equal(sl->head, sn0, "head sl");
    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, sn2, "next sn0");
    expect_ptr_equal(sn2->prev, sn0, "prev sn2");
    expect_ptr_equal(sn2->next, NULL, "next sn2");
    expect_ptr_equal(sl->tail, sn2, "tail sl");

    expect_ptr_equal(sn1->prev, NULL, "prev sn1");
    expect_ptr_equal(sn1->next, NULL, "next sn1");

    Stack_node_destroy(sn1);
    Stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_head()
{
    test_name(__func__);

    Stack_list* sl = NULL;
    Stack_list_create(&sl);

    Stack_node* sn0 = NULL;
    Stack_node_create(&sn0);

    Stack_node* sn1 = NULL;
    Stack_node_create(&sn1);

    Stack_node* sn2 = NULL;
    Stack_node_create(&sn2);

    Stack_list_add(sl, sn0);
    Stack_list_add(sl, sn1);
    Stack_list_add(sl, sn2);

    expect_ptr_equal(sl->head, sn0, "head sl");
    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, sn1, "next sn0");
    expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    expect_ptr_equal(sn1->next, sn2, "next sn1");
    expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    expect_ptr_equal(sn2->next, NULL, "next sn2");
    expect_ptr_equal(sl->tail, sn2, "tail sl");

    Stack_list_remove(sl, sn0);

    expect_ptr_equal(sl->head, sn1, "head sl");
    expect_ptr_equal(sn1->prev, NULL, "prev sn1");
    expect_ptr_equal(sn1->next, sn2, "next sn1");
    expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    expect_ptr_equal(sn2->next, NULL, "next sn2");
    expect_ptr_equal(sl->tail, sn2, "tail sl");

    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, NULL, "next sn0");

    Stack_node_destroy(sn0);
    Stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_tail()
{
    test_name(__func__);

    Stack_list* sl = NULL;
    Stack_list_create(&sl);

    Stack_node* sn0 = NULL;
    Stack_node_create(&sn0);

    Stack_node* sn1 = NULL;
    Stack_node_create(&sn1);

    Stack_node* sn2 = NULL;
    Stack_node_create(&sn2);

    Stack_list_add(sl, sn0);
    Stack_list_add(sl, sn1);
    Stack_list_add(sl, sn2);

    expect_ptr_equal(sl->head, sn0, "head sl");
    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, sn1, "next sn0");
    expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    expect_ptr_equal(sn1->next, sn2, "next sn1");
    expect_ptr_equal(sn2->prev, sn1, "prev sn2");
    expect_ptr_equal(sn2->next, NULL, "next sn2");
    expect_ptr_equal(sl->tail, sn2, "tail sl");

    Stack_list_remove(sl, sn2);

    expect_ptr_equal(sl->head, sn0, "head sl");
    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, sn1, "next sn0");
    expect_ptr_equal(sn1->prev, sn0, "prev sn1");
    expect_ptr_equal(sn1->next, NULL, "next sn1");
    expect_ptr_equal(sl->tail, sn1, "tail sl");

    expect_ptr_equal(sn2->prev, NULL, "prev sn2");
    expect_ptr_equal(sn2->next, NULL, "next sn2");

    Stack_node_destroy(sn2);
    Stack_list_destroy(sl);
}

void test_match_tools_stack_list_remove_single()
{
    test_name(__func__);

    Stack_list* sl = NULL;
    Stack_list_create(&sl);

    Stack_node* sn0 = NULL;
    Stack_node_create(&sn0);

    Stack_list_add(sl, sn0);

    expect_ptr_equal(sl->head, sn0, "head sl");
    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, NULL, "next sn0");
    expect_ptr_equal(sl->tail, sn0, "tail sl");

    Stack_list_remove(sl, sn0);

    expect_ptr_equal(sl->head, NULL, "head sl");
    expect_ptr_equal(sl->tail, NULL, "tail sl");

    expect_ptr_equal(sn0->prev, NULL, "prev sn0");
    expect_ptr_equal(sn0->next, NULL, "next sn0");

    Stack_node_destroy(sn0);
    Stack_list_destroy(sl);
}

void test_match_tools_clone()
{
    test_name(__func__);

    Match_task_stack* mts = NULL;
    Match_task_stack_create(&mts, NULL);

    Match_task* mt0 = NULL;
    Match_task_create(&mt0, NULL);

    Match_task* mt1 = NULL;
    Match_task_create(&mt1, NULL);

    Match_task* mt2 = NULL;
    Match_task_create(&mt2, mt0);

    Match_task* mt3 = NULL;
    Match_task_create(&mt3, mt1);

    Match_task_stack_push(mts, mt0);
    Match_task_stack_push(mts, mt1);
    Match_task_stack_push(mts, mt2);
    Match_task_stack_push(mts, mt3);

    struct buffer* bf = NULL;
    buffer_create(&bf);

    struct hash_table ht;
    hash_table_init(&ht, 32);

    Match_task_stack* new_mts = Match_task_stack_clone(mts, &ht, NULL);
    Match_task* new_mt3 = new_mts->top;
    Match_task* new_mt2 = new_mt3->next;
    Match_task* new_mt1 = new_mt2->next;
    Match_task* new_mt0 = new_mt1->next;

    expect_ptr_equal(new_mts->top, new_mt3, "top new_mts");
    expect_ptr_equal(new_mt3->prev, NULL, "prev new_mt3");
    expect_ptr_equal(new_mt3->next, new_mt2, "next new_mt3");
    expect_ptr_equal(new_mt2->prev, new_mt3, "prev new_mt2");
    expect_ptr_equal(new_mt2->next, new_mt1, "next new_mt2");
    expect_ptr_equal(new_mt1->prev, new_mt2, "prev new_mt1");
    expect_ptr_equal(new_mt1->next, new_mt0, "next new_mt1");
    expect_ptr_equal(new_mt0->prev, new_mt1, "prev new_mt0");
    expect_ptr_equal(new_mt0->next, NULL, "next new_mt0");
    expect_ptr_equal(mts->bottom, mt0, "bottom mts");

    expect_ptr_equal(new_mt2->parent, new_mt0, "parent new_mt2");
    expect_ptr_equal(new_mt3->parent, new_mt1, "parent new_mt3");

    Match_task_stack_destroy(mts);
    Match_task_stack_destroy(new_mts);
    buffer_destroy(bf);
    free(bf);
    hash_table_destroy(&ht);
}

void test_match_tools_stack_node_clone()
{
    test_name(__func__);

    struct buffer* bf = NULL;
    buffer_create(&bf);
    buffer_copy_str(bf, "abc");

    Stack_node* sn0 = NULL;
    Stack_node_create(&sn0);
    Hash_map_size_t_add(&sn0->groups, 0, bf);

    Stack_node* sn1 = Stack_node_clone(sn0);

    struct buffer* bf2 = Hash_map_size_t_get(&sn0->groups, 0);
    expect_ptr(bf2, "ptr bf2");
    expect_str(bf2, "abc", "str bf2");

    Stack_node_destroy(sn0);
    Stack_node_destroy(sn1);
}

void test_match_tools_stack_priority()
{
    test_name(__func__);

    Stack_list* sl = NULL;
    Stack_list_create(&sl);

    Stack_node* sn30 = NULL;
    Stack_node_create(&sn30);
    sn30->priority = 30;

    Stack_node* sn20 = NULL;
    Stack_node_create(&sn20);
    sn20->priority = 20;

    Stack_node* sn10 = NULL;
    Stack_node_create(&sn10);
    sn10->priority = 10;

    Stack_node* sn40 = NULL;
    Stack_node_create(&sn40);
    sn40->priority = 40;

    Stack_list_add(sl, sn30);
    Stack_list_add(sl, sn20);
    Stack_list_add(sl, sn10);
    Stack_list_add(sl, sn40);

    Stack_node* x = NULL;

    x = sl->tail;
    expect_size_t_equal(x->priority, 40, "priority 40");
    Stack_list_remove(sl, x);
    Stack_node_destroy(x);

    x = sl->tail;
    expect_size_t_equal(x->priority, 30, "priority 40");
    Stack_list_remove(sl, x);
    Stack_node_destroy(x);

    x = sl->tail;
    expect_size_t_equal(x->priority, 20, "priority 40");
    Stack_list_remove(sl, x);
    Stack_node_destroy(x);

    x = sl->tail;
    expect_size_t_equal(x->priority, 10, "priority 40");
    Stack_list_remove(sl, x);
    Stack_node_destroy(x);

    Stack_list_destroy(sl);
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