#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_dom.h"
#include "test_parse.h"

int main()
{
    test_lex();
    test_dom();
    test_parse();
    print_results();
    return 0;
}