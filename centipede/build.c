#include "parse_data.h"
#include "value.h"

Cent_value* Cent_build_dispatch(Cent_ast* n);

Cent_value* Cent_build(Cent_parse_result* pr)
{
    Cent_value* value = NULL;

    if (!pr->errors->head) {
        Cent_ast* last = pr->root->tail;
        if (last) {
            value = Cent_build_dispatch(last);
        }
    }

    return value;
}

Cent_value* Cent_build_dispatch(Cent_ast* n)
{
}

Cent_value* Cent_build_value(Cent_ast* ast)
{
    Cent_value* value = NULL;
    Cent_ast* p = ast->head;
    while (p) {
        p = p->next;
    }
}