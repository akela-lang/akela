#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_dom.h"

int main()
{
    test_lex();
    test_dom();
    print_results();
    return 0;
}