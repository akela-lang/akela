#include "zinc/unit_test.h"
#include "test_ast_node.h"
#include "test_compile.h"
#include "test_match.h"
#include "test_match_tools.h"

int main()
{
    test_ast_node();
    test_compile();
    test_match_tools();
    test_match();
    print_results();
    return 0;
}