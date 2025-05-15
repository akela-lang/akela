#include "unit_ast_node.h"
#include "test_compile.h"
#include "test_match.h"
#include "test_match_tools.h"
#include "zinc/test.h"
#include "zinc/os.h"
#if IS_WIN
#include <Windows.h>
#endif

void CobUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CobUnit_ast_node);
        // test_compile();
        // test_match_tools();
        // test_match();

        return;
    }

#if IS_WIN
    SetConsoleOutputCP(CP_UTF8);
#endif
    Zinc_test_perform(test);
}