#include "zinc/unit_test.h"
#include "zinc/priority_queue.h"

void test_priority_queue_add()
{
    Zinc_test_name(__func__);

    Zinc_priority_queue* pq = NULL;
    Zinc_priority_queue_create(&pq);

    Zinc_priority_task* t22 = NULL;
    Zinc_priority_task_create(&t22);
    t22->priority = 22;

    Zinc_priority_task* t54 = NULL;
    Zinc_priority_task_create(&t54);
    t54->priority = 54;

    Zinc_priority_task* t30 = NULL;
    Zinc_priority_task_create(&t30);
    t30->priority = 30;

    Zinc_priority_task* t15 = NULL;
    Zinc_priority_task_create(&t15);
    t15->priority = 15;

    Zinc_priority_task* t60 = NULL;
    Zinc_priority_task_create(&t60);
    t60->priority = 60;

    Zinc_priority_queue_add(pq, t22);
    Zinc_priority_queue_add(pq, t54);
    Zinc_priority_queue_add(pq, t30);
    Zinc_priority_queue_add(pq, t15);
    Zinc_priority_queue_add(pq, t60);

    Zinc_expect_ptr_equal(pq->head, t15, "head pq");
    Zinc_expect_ptr_equal(t15->prev, NULL, "prev t15");
    Zinc_expect_ptr_equal(t15->next, t22, "next t15");
    Zinc_expect_ptr_equal(t22->prev, t15, "prev t22");
    Zinc_expect_ptr_equal(t22->next, t30, "next t22");
    Zinc_expect_ptr_equal(t30->prev, t22, "prev t30");
    Zinc_expect_ptr_equal(t30->next, t54, "next t30");
    Zinc_expect_ptr_equal(t54->prev, t30, "prev t54");
    Zinc_expect_ptr_equal(t54->next, t60, "next t54");
    Zinc_expect_ptr_equal(t60->prev, t54, "prev t60");
    Zinc_expect_ptr_equal(t60->next, NULL, "next t60");
    Zinc_expect_ptr_equal(pq->tail, t60, "tail pq");

    Zinc_priority_queue_destroy(pq);
    free(pq);
}

void test_priority_queue_remove()
{
    Zinc_test_name(__func__);

    Zinc_priority_queue* pq = NULL;
    Zinc_priority_queue_create(&pq);

    Zinc_priority_task* t22 = NULL;
    Zinc_priority_task_create(&t22);
    t22->priority = 22;

    Zinc_priority_task* t54 = NULL;
    Zinc_priority_task_create(&t54);
    t54->priority = 54;

    Zinc_priority_task* t30 = NULL;
    Zinc_priority_task_create(&t30);
    t30->priority = 30;

    Zinc_priority_task* t15 = NULL;
    Zinc_priority_task_create(&t15);
    t15->priority = 15;

    Zinc_priority_task* t60 = NULL;
    Zinc_priority_task_create(&t60);
    t60->priority = 60;

    Zinc_priority_queue_add(pq, t22);
    Zinc_priority_queue_add(pq, t54);
    Zinc_priority_queue_add(pq, t30);
    Zinc_priority_queue_add(pq, t15);
    Zinc_priority_queue_add(pq, t60);

    Zinc_expect_ptr_equal(Zinc_priority_queue_pop(pq), t60, "pop 0");
    Zinc_expect_ptr_equal(Zinc_priority_queue_pop(pq), t54, "pop 1");
    Zinc_expect_ptr_equal(Zinc_priority_queue_pop(pq), t30, "pop 2");
    Zinc_expect_ptr_equal(Zinc_priority_queue_pop(pq), t22, "pop 3");
    Zinc_expect_ptr_equal(Zinc_priority_queue_pop(pq), t15, "pop 4");
    Zinc_expect_ptr_equal(Zinc_priority_queue_pop(pq), NULL, "pop 5");

    Zinc_priority_queue_destroy(pq);
    free(t22);
    free(t54);
    free(t30);
    free(t15);
    free(t60);
    free(pq);
}

void test_priority_queue()
{
    test_priority_queue_add();
    test_priority_queue_remove();
}