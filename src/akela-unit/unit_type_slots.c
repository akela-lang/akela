#include "akela/type_slots.h"
#include "zinc/expect.h"
#include "zinc/test.h"

void AkeUnit_TypeSlotsProcess(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_TypeSlots slots;
    Ake_TypeSlotsInit(&slots);

    Zinc_expect_size_t_equal(test, Ake_TypeSlotsCount(&slots), 0, "0 slot count");
    Zinc_expect_size_t_equal(test, slots.index_list.count, 0, "0 pos count");

    Ake_Type* t0 = NULL;
    Ake_TypeCreate(&t0);
    Ake_TypeSet(t0, AKE_TYPE_INTEGER);
    t0->data.integer.bit_count = 32;
    Ake_TypeSlotsProcess(&slots, t0);

    Zinc_expect_size_t_equal(test, Ake_TypeSlotsCount(&slots), 1, "1 slot count");
    Zinc_expect_size_t_equal(test, slots.index_list.count, 1, "1 pos count");
    size_t slot0 = Ake_TypeSlotsGetSlot(&slots, t0);
    Zinc_expect_size_t_equal(test, slot0, 0, "1-0 index");
    Ake_Type* t00 = Ake_TypeSlotsGetType(&slots, slot0);
    Zinc_expect_ptr_equal(test, t00, t0, "1-0 type");

    Ake_Type* t1 = NULL;
    Ake_TypeCreate(&t1);
    Ake_TypeSet(t1, AKE_TYPE_INTEGER);
    t1->data.integer.bit_count = 32;
    Ake_TypeSlotsProcess(&slots, t1);

    Zinc_expect_size_t_equal(test, Ake_TypeSlotsCount(&slots), 1, "2 slot count");
    Zinc_expect_size_t_equal(test, slots.index_list.count, 2, "2 pos count");

    slot0 = Ake_TypeSlotsGetSlot(&slots, t0);
    Zinc_expect_size_t_equal(test, slot0, 0, "2 0 index");
    t00 = Ake_TypeSlotsGetType(&slots, slot0);
    Zinc_expect_ptr_equal(test, t00, t0, "2 0 type");

    size_t slot1 = Ake_TypeSlotsGetSlot(&slots, t1);
    Zinc_expect_size_t_equal(test, slot1, 0, "2 1 index");
    Ake_Type* t01 = Ake_TypeSlotsGetType(&slots, slot1);
    Zinc_expect_ptr_equal(test, t01, t0, "2 1 type");

    Ake_Type* t2 = NULL;
    Ake_TypeCreate(&t2);
    Ake_TypeSet(t2, AKE_TYPE_NATURAL);
    t2->data.integer.bit_count = 32;
    Ake_TypeSlotsProcess(&slots, t2);

    Zinc_expect_size_t_equal(test, Ake_TypeSlotsCount(&slots), 2, "3 slot count");
    Zinc_expect_size_t_equal(test, slots.index_list.count, 3, "3 pos count");

    slot0 = Ake_TypeSlotsGetSlot(&slots, t0);
    Zinc_expect_size_t_equal(test, slot0, 0, "3 0 index");
    t00 = Ake_TypeSlotsGetType(&slots, slot0);
    Zinc_expect_ptr_equal(test, t00, t0, "3 0 type");

    slot1 = Ake_TypeSlotsGetSlot(&slots, t1);
    Zinc_expect_size_t_equal(test, slot1, 0, "3 1 index");
    t01 = Ake_TypeSlotsGetType(&slots, slot1);
    Zinc_expect_ptr_equal(test, t01, t0, "3 1 type");

    size_t slot2 = Ake_TypeSlotsGetSlot(&slots, t2);
    Zinc_expect_size_t_equal(test, slot2, 1, "3 2 index");
    Ake_Type* t02 = Ake_TypeSlotsGetType(&slots, slot2);
    Zinc_expect_ptr_equal(test, t02, t2, "3 2 type");

    Ake_Type* t3 = NULL;
    Ake_TypeCreate(&t3);
    Ake_TypeSet(t3, AKE_TYPE_NATURAL);
    t3->data.integer.bit_count = 32;
    Ake_TypeSlotsProcess(&slots, t3);

    Zinc_expect_size_t_equal(test, Ake_TypeSlotsCount(&slots), 2, "4 slot count");
    Zinc_expect_size_t_equal(test, slots.index_list.count, 4, "4 pos count");

    slot0 = Ake_TypeSlotsGetSlot(&slots, t0);
    Zinc_expect_size_t_equal(test, slot0, 0, "4 0 index");
    t00 = Ake_TypeSlotsGetType(&slots, slot0);
    Zinc_expect_ptr_equal(test, t00, t0, "4 0 type");

    slot1 = Ake_TypeSlotsGetSlot(&slots, t1);
    Zinc_expect_size_t_equal(test, slot1, 0, "4 1 index");
    t01 = Ake_TypeSlotsGetType(&slots, slot1);
    Zinc_expect_ptr_equal(test, t01, t0, "4 1 type");

    slot2 = Ake_TypeSlotsGetSlot(&slots, t2);
    Zinc_expect_size_t_equal(test, slot2, 1, "4 2 index");
    t02 = Ake_TypeSlotsGetType(&slots, slot2);
    Zinc_expect_ptr_equal(test, t02, t2, "4 2 type");

    size_t slot3 = Ake_TypeSlotsGetSlot(&slots, t3);
    Zinc_expect_size_t_equal(test, slot3, 1, "4 3 index");
    Ake_Type* t03 = Ake_TypeSlotsGetType(&slots, slot3);
    Zinc_expect_ptr_equal(test, t03, t2, "4 3 type");

    Ake_TypeSlotsDestroy(&slots);
}

void AkeUnit_TypeSlots(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_TypeSlotsProcess);

        return;
    }

    Zinc_test_perform(test);
}