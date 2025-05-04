#include "zinc/priority_queue.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_priority_queue_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_test_expect_ptr_equal(test, pq->head, t15, "head pq");
    Zinc_test_expect_ptr_equal(test, t15->prev, NULL, "prev t15");
    Zinc_test_expect_ptr_equal(test, t15->next, t22, "next t15");
    Zinc_test_expect_ptr_equal(test, t22->prev, t15, "prev t22");
    Zinc_test_expect_ptr_equal(test, t22->next, t30, "next t22");
    Zinc_test_expect_ptr_equal(test, t30->prev, t22, "prev t30");
    Zinc_test_expect_ptr_equal(test, t30->next, t54, "next t30");
    Zinc_test_expect_ptr_equal(test, t54->prev, t30, "prev t54");
    Zinc_test_expect_ptr_equal(test, t54->next, t60, "next t54");
    Zinc_test_expect_ptr_equal(test, t60->prev, t54, "prev t60");
    Zinc_test_expect_ptr_equal(test, t60->next, NULL, "next t60");
    Zinc_test_expect_ptr_equal(test, pq->tail, t60, "tail pq");

    Zinc_priority_queue_destroy(pq);
    free(pq);
}

void Zinc_unit_priority_queue_pop_highest(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_highest(pq), t60, "pop 0");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_highest(pq), t54, "pop 1");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_highest(pq), t30, "pop 2");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_highest(pq), t22, "pop 3");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_highest(pq), t15, "pop 4");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_highest(pq), NULL, "pop 5");

    Zinc_priority_queue_destroy(pq);
    free(t22);
    free(t54);
    free(t30);
    free(t15);
    free(t60);
    free(pq);
}

void Zinc_unit_priority_queue_pop_lowest(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_lowest(pq), t15, "pop 0");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_lowest(pq), t22, "pop 1");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_lowest(pq), t30, "pop 2");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_lowest(pq), t54, "pop 3");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_lowest(pq), t60, "pop 4");
    Zinc_test_expect_ptr_equal(test, Zinc_priority_queue_pop_lowest(pq), NULL, "pop 5");

    Zinc_priority_queue_destroy(pq);
    free(t22);
    free(t54);
    free(t30);
    free(t15);
    free(t60);
    free(pq);
}

void Zinc_unit_priority_queue(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_priority_queue_add);
        Zinc_test_register(test, Zinc_unit_priority_queue_pop_highest);
        Zinc_test_register(test, Zinc_unit_priority_queue_pop_lowest);

        return;
    }

    Zinc_test_perform(test);
}