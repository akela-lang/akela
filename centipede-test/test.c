#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_value.h"

int main()
{
    test_lex();
    test_value();
    print_results();
    return 0;
}