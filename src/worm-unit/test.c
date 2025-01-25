#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_ast.h"

int main()
{
    test_lex();
    test_ast();
    Zinc_print_results();
    return 0;
}
