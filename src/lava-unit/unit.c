#include "unit_lex.h"
#include "zinc/unit_test.h"
#include "unit_parse.h"

int main()
{
    unit_lex();
    unit_parse();
    Zinc_print_results();
    return 0;
}