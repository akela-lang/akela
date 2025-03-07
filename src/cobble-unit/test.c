#include "zinc/unit_test.h"
#include "test_ast_node.h"
#include "test_compile.h"
#include "test_match.h"
#include "test_match_tools.h"
#include "zinc/os.h"
#if IS_WIN
#include <Windows.h>
#endif

int main()
{
#if IS_WIN
    SetConsoleOutputCP(CP_UTF8);
#endif
    test_ast_node();
    test_compile();
    test_match_tools();
    test_match();
    Zinc_print_results();
    return 0;
}