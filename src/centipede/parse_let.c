#include "ast.h"
#include "parse_tools.h"
#include "parse_expr.h"

Cent_ast* Cent_parse_let(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_let;

    Cent_token* let = NULL;
    Cent_match(pd, Cent_token_let, "expected let", &let, n);

    Cent_ast* id_node = NULL;
    Cent_ast_create(&id_node);

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, id_node);
    if (id) {
        buffer_copy(&id->value, &id_node->text);
        Cent_token_destroy(id);
        free(id);
    }
    Cent_ast_add(n, id_node);

    Cent_token* eq = NULL;
    Cent_match(pd, Cent_token_equal, "expected equal", &eq, n);
    Cent_token_destroy(eq);
    free(eq);

    Cent_ast* a = Cent_parse_expr(pd);
    Cent_ast_add(n, a);

    return n;
}