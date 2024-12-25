#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_value.h"
#include "test_ast.h"
#include "test_parse.h"

int main()
{
    test_lex();
    test_value();
    test_ast();
    test_parse();
    print_results();
    return 0;
}