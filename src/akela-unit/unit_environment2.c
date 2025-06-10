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

    Ake_Environment* env0 = NULL;
    Ake_EnvironmentCreate(&env0, NULL);

    Ake_symbol* t0 = NULL;
    Ake_symbol_create(&t0);

    Ake_symbol* t1 = NULL;
    Ake_symbol_create(&t1);

    Ake_symbol* t2 = NULL;
    Ake_symbol_create(&t2);

    Ake_EnvironmentAddStr(env0, "t0", t0, 0);
    Ake_EnvironmentAddStr(env0, "t1", t1, 0);
    Ake_EnvironmentAddStr(env0, "t2", t2, 0);

    Ake_Environment* env1 = NULL;
    Ake_EnvironmentCreate(&env1, env0);

    Ake_symbol* sym0 = NULL;
    Ake_symbol_create(&sym0);

    Ake_symbol* sym1 = NULL;
    Ake_symbol_create(&sym1);

    Ake_symbol* sym2 = NULL;
    Ake_symbol_create(&sym2);

    Ake_EnvironmentAddStr(env1, "sym0", sym0, 10);
    Ake_EnvironmentAddStr(env1, "sym1", sym1, 20);
    Ake_EnvironmentAddStr(env1, "sym2", sym2, 30);

    Ake_symbol* sym = NULL;

    sym = Ake_EnvironmentGetStr(env1, "sym0", 9);
    Zinc_expect_null(test, sym, "sym0 seq 9");

    sym = Ake_EnvironmentGetStr(env1, "sym0", 10);
    Zinc_expect_ptr_equal(test, sym, sym0, "sym0 seq 10");

    sym = Ake_EnvironmentGetStr(env1, "sym0", 11);
    Zinc_expect_ptr_equal(test, sym, sym0, "sym0 seq 11");

    sym = Ake_EnvironmentGetStr(env1, "sym1", 19);
    Zinc_expect_null(test, sym, "sym1 seq 19");

    sym = Ake_EnvironmentGetStr(env1, "sym1", 20);
    Zinc_expect_ptr_equal(test, sym, sym1, "sym1 seq 20");

    sym = Ake_EnvironmentGetStr(env1, "sym1", 21);
    Zinc_expect_ptr_equal(test, sym, sym1, "sym1 seq 21");

    sym = Ake_EnvironmentGetStr(env1, "sym2", 29);
    Zinc_expect_null(test, sym, "sym1 seq 29");

    sym = Ake_EnvironmentGetStr(env1, "sym2", 30);
    Zinc_expect_ptr_equal(test, sym, sym2, "sym1 seq 30");

    sym = Ake_EnvironmentGetStr(env1, "sym2", 31);
    Zinc_expect_ptr_equal(test, sym, sym2, "sym1 seq 31");

    sym = Ake_EnvironmentGetStr(env1, "t0", 0);
    Zinc_expect_ptr_equal(test, sym, t0, "t0");

    sym = Ake_EnvironmentGetStr(env1, "t1", 0);
    Zinc_expect_ptr_equal(test, sym, t1, "t1");

    sym = Ake_EnvironmentGetStr(env1, "t2", 0);
    Zinc_expect_ptr_equal(test, sym, t2, "t2");

    Ake_EnvironmentDestroy(env1);
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