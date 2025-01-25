#include "parse_data.h"
#include "parse_tools.h"
#include "parse_stmts.h"

Worm_ast* Worm_parse(Worm_parse_data* pd)
{
    Worm_ast* root = Worm_parse_stmts(pd);
    if (pd->lookahead && pd->lookahead->type != Worm_token_type_eof) {
        Zinc_error_list_set(
            pd->errors,
            &pd->lookahead->loc,
            "token not processed %s",
            Worm_token_name(pd->lookahead->type));
    }

    if (pd->lookahead) {
        Worm_token_destroy(pd->lookahead);
        free(pd->lookahead);
    }

    return root;
}

