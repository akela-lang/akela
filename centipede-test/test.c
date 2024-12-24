#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_value.h"
#include "test_ast.h"

int main()
{
    test_lex();
    test_value();
    test_ast();
    print_results();
    return 0;
}