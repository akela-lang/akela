#include "zinc-unit/unit.h"
#include "lava-unit/unit.h"
#include "json-unit/unit.h"
#include "dataframe-unit/unit.h"
#include "coverage-unit/unit.h"
#include "cobble-unit/unit.h"
#include "centipede-unit/unit.h"
#include "akela-unit/unit.h"
#include "akela-llvm-unit/unit.h"
#include "akela-parse-test/test.h"
#include "akela-run-test/run.h"

void Test_test(Zinc_test* test);

int main()
{
    Zinc_test test;
    Zinc_test_init(&test);

    test.dry_run = true;
    Test_test(&test);

    test.dry_run = false;
    Test_test(&test);

    Zinc_test_stat stat;
    Zinc_test_stat_init(&stat);

    Zinc_test_count(&test, &stat);
    Zinc_test_print(&stat);

    Zinc_test_destroy(&test);

    return 0;
}

void Test_test(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, "Test");

        Zinc_test_register(test, Zinc_unit);
        Zinc_test_register(test, Lava_unit);
        Zinc_test_register(test, Json_unit);
        Zinc_test_register(test, DfUnit);
        Zinc_test_register(test, CoverUnit);
        Zinc_test_register(test, CobUnit);
        Zinc_test_register(test, CentUnit);
        Zinc_test_register(test, AkeUnit);
        Zinc_test_register(test, AkeLlvmUnit);
        Zinc_test_register(test, Apt);
        Zinc_test_register(test, Art);

        return;
    }

    Zinc_test_perform(test);
}