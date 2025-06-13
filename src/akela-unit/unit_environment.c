#include "zinc/test.h"
#include "zinc/expect.h"
#include "akela/environment.h"

void AkeUnit_EnvironmentAdd(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_symbol_table* st;
    Ake_symbol_table_create(&st);

    Ake_symbol* t0 = NULL;
    Ake_symbol_create(&t0);

    Ake_symbol* t1 = NULL;
    Ake_symbol_create(&t1);

    Ake_symbol* t2 = NULL;
    Ake_symbol_create(&t2);

    Ake_begin_environment(st, NULL);
    Ake_EnvironmentAddStr(st->top, "t0", t0, AKE_SEQ_DEFAULT);
    Ake_EnvironmentAddStr(st->top, "t1", t1, AKE_SEQ_DEFAULT);
    Ake_EnvironmentAddStr(st->top, "t2", t2, AKE_SEQ_DEFAULT);

    Ake_symbol* sym0 = NULL;
    Ake_symbol_create(&sym0);

    Ake_symbol* sym1 = NULL;
    Ake_symbol_create(&sym1);

    Ake_symbol* sym2 = NULL;
    Ake_symbol_create(&sym2);

    Ake_begin_environment(st, NULL);
    Ake_EnvironmentAddStr(st->top, "sym0", sym0, 10);
    Ake_EnvironmentAddStr(st->top, "sym1", sym1, 20);
    Ake_EnvironmentAddStr(st->top, "sym2", sym2, 30);

    Ake_symbol* sym = NULL;

    sym = Ake_EnvironmentGetStr(st->top, "sym0", 9);
    Zinc_expect_ptr_equal(test, sym, NULL, "sym0 seq 9");

    sym = Ake_EnvironmentGetStr(st->top, "sym0", 10);
    Zinc_expect_ptr_equal(test, sym, sym0, "sym0 seq 10");

    sym = Ake_EnvironmentGetStr(st->top, "sym0", 11);
    Zinc_expect_ptr_equal(test, sym, sym0, "sym0 seq 11");

    sym = Ake_EnvironmentGetStr(st->top, "sym1", 19);
    Zinc_expect_ptr_equal(test, sym, NULL, "sym1 seq 19");

    sym = Ake_EnvironmentGetStr(st->top, "sym1", 20);
    Zinc_expect_ptr_equal(test, sym, sym1, "sym1 seq 20");

    sym = Ake_EnvironmentGetStr(st->top, "sym1", 21);
    Zinc_expect_ptr_equal(test, sym, sym1, "sym1 seq 21");

    sym = Ake_EnvironmentGetStr(st->top, "sym2", 29);
    Zinc_expect_ptr_equal(test, sym, NULL, "sym1 seq 29");

    sym = Ake_EnvironmentGetStr(st->top, "sym2", 30);
    Zinc_expect_ptr_equal(test, sym, sym2, "sym1 seq 30");

    sym = Ake_EnvironmentGetStr(st->top, "sym2", 31);
    Zinc_expect_ptr_equal(test, sym, sym2, "sym1 seq 31");

    sym = Ake_EnvironmentGetStr(st->top, "t0", AKE_SEQ_DEFAULT);
    Zinc_expect_ptr_equal(test, sym, t0, "t0");

    sym = Ake_EnvironmentGetStr(st->top, "t1", AKE_SEQ_DEFAULT);
    Zinc_expect_ptr_equal(test, sym, t1, "t1");

    sym = Ake_EnvironmentGetStr(st->top, "t2", AKE_SEQ_DEFAULT);
    Zinc_expect_ptr_equal(test, sym, t2, "t2");

    Ake_symbol_table_destroy(st);
    free(st);
}

void AkeUnit_EnvironmentAdd2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_symbol_table* st = NULL;
    Ake_symbol_table_create(&st);

    Ake_symbol* sym0 = NULL;
    Ake_symbol_create(&sym0);

    Ake_symbol* sym1 = NULL;
    Ake_symbol_create(&sym1);

    Ake_symbol* sym2 = NULL;
    Ake_symbol_create(&sym2);

    Ake_begin_environment(st, NULL);
    Ake_EnvironmentAddStr(st->top, "a", sym0, AKE_SEQ_DEFAULT);
    Ake_EnvironmentAddStr(st->top, "a", sym1, AKE_SEQ_DEFAULT);
    Ake_EnvironmentAddStr(st->top, "a", sym2, AKE_SEQ_DEFAULT);

    size_t val = Ake_HashCalcStr("a", AKE_ENVIRONMENT_SIZE);
    Ake_EnvironmentEntryList* list = &st->top->buckets[val];

    Ake_EnvironmentEntry* ent0 = list->head;
    if (!Zinc_expect_ptr(test, ent0, "ptr ent0")) {
        return Zinc_assert();
    }

    Ake_EnvironmentEntry* ent1 = ent0->next;
    if (!Zinc_expect_ptr(test, ent1, "ptr ent1")) {
        return Zinc_assert();
    }

    Ake_EnvironmentEntry* ent2 = ent1->next;
    if (!Zinc_expect_ptr(test, ent2, "ptr ent2")) {
        return Zinc_assert();
    }

    Zinc_expect_ptr_equal(test, list->head, ent0, "head list");

    Zinc_expect_ptr_equal(test, ent0->sym, sym0, "sym0");
    Zinc_expect_ptr_equal(test, ent0->prev, NULL, "prev ent0");
    Zinc_expect_ptr_equal(test, ent0->next, ent1, "next ent0");

    Zinc_expect_ptr_equal(test, ent1->sym, sym1, "sym1");
    Zinc_expect_ptr_equal(test, ent1->prev, ent0, "prev ent1");
    Zinc_expect_ptr_equal(test, ent1->next, ent2, "next ent1");

    Zinc_expect_ptr_equal(test, ent2->sym, sym2, "sym2");
    Zinc_expect_ptr_equal(test, ent2->prev, ent1, "prev ent2");
    Zinc_expect_ptr_equal(test, ent2->next, NULL, "next ent2");

    Zinc_expect_ptr_equal(test, list->tail, ent2, "tail list");

    Ake_symbol_table_destroy(st);
    free(st);
}

void AkeUnit_Environment(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_EnvironmentAdd);
        Zinc_test_register(test, AkeUnit_EnvironmentAdd2);

        return;
    }

    Zinc_test_perform(test);
}