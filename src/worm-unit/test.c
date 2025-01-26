#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_ast.h"
#include "test_parse.h"

int main()
{
    test_lex();
    test_ast();
    test_parse();
    Zinc_print_results();
    return 0;
}
