#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_value.h"
#include "test_ast.h"
#include "test_parse.h"
#include "test_build.h"

int main()
{
    test_lex();
    test_value();
    test_ast();
    test_parse();
    test_build();
    print_results();
    return 0;
}