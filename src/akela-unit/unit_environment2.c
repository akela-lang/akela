#include "zinc/test.h"
#include "zinc/expect.h"
#include "akela/environment2.h"

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

    Ake_EnvironmentBegin(st);
    Ake_EnvironmentAddStr(st, "t0", t0);
    Ake_EnvironmentAddStr(st, "t1", t1);
    Ake_EnvironmentAddStr(st, "t2", t2);

    Ake_symbol* sym0 = NULL;
    Ake_symbol_create(&sym0);

    Ake_symbol* sym1 = NULL;
    Ake_symbol_create(&sym1);

    Ake_symbol* sym2 = NULL;
    Ake_symbol_create(&sym2);

    Ake_EnvironmentBegin(st);
    Ake_EnvironmentAddStr(st, "sym0", sym0);
    Ake_EnvironmentAddStr(st, "sym1", sym1);
    Ake_EnvironmentAddStr(st, "sym2", sym2);

    Ake_symbol* sym = NULL;

    sym = Ake_EnvironmentGetStr(st, "sym0");
    Zinc_expect_ptr_equal(test, sym, sym0, "sym0 seq 9");

    sym = Ake_EnvironmentGetStr(st, "sym0");
    Zinc_expect_ptr_equal(test, sym, sym0, "sym0 seq 10");

    sym = Ake_EnvironmentGetStr(st, "sym0");
    Zinc_expect_ptr_equal(test, sym, sym0, "sym0 seq 11");

    sym = Ake_EnvironmentGetStr(st, "sym1");
    Zinc_expect_ptr_equal(test, sym, sym1, "sym1 seq 19");

    sym = Ake_EnvironmentGetStr(st, "sym1");
    Zinc_expect_ptr_equal(test, sym, sym1, "sym1 seq 20");

    sym = Ake_EnvironmentGetStr(st, "sym1");
    Zinc_expect_ptr_equal(test, sym, sym1, "sym1 seq 21");

    sym = Ake_EnvironmentGetStr(st, "sym2");
    Zinc_expect_ptr_equal(test, sym, sym2, "sym1 seq 29");

    sym = Ake_EnvironmentGetStr(st, "sym2");
    Zinc_expect_ptr_equal(test, sym, sym2, "sym1 seq 30");

    sym = Ake_EnvironmentGetStr(st, "sym2");
    Zinc_expect_ptr_equal(test, sym, sym2, "sym1 seq 31");

    sym = Ake_EnvironmentGetStr(st, "t0");
    Zinc_expect_ptr_equal(test, sym, t0, "t0");

    sym = Ake_EnvironmentGetStr(st, "t1");
    Zinc_expect_ptr_equal(test, sym, t1, "t1");

    sym = Ake_EnvironmentGetStr(st, "t2");
    Zinc_expect_ptr_equal(test, sym, t2, "t2");

    Ake_symbol_table_destroy(st);
}

void AkeUnit_Environment(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_EnvironmentAdd);

        return;
    }

    Zinc_test_perform(test);
}