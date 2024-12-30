#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include <akela/parse_tools.h>
#include "base.h"
#include "token.h"
#include "update_types.h"
#include "parse_stmts.h"

Cent_parse_result Cent_parse(Cent_parse_data* pd)
{
    pd->top = Cent_base_create();
    Cent_ast* root = Cent_parse_stmts(pd);
    if (pd->lookahead->type != Cent_token_eof) {
        error_list_set(
            pd->errors,
            &pd->lookahead->loc,
            "unhandled token: %s",
            Cent_token_name(pd->lookahead->type));
    }
    /* test case: test_parse_unhandled_token */

    if (pd->lookahead) {
        Cent_token* t = pd->lookahead;
        pd->lookahead = NULL;
        Cent_token_destroy(t);
        free(t);
    }

    Cent_parse_result pr;
    Cent_parse_result_init(&pr);
    pr.errors = pd->errors;
    pr.base = pd->top;
    pr.root = root;

    if (!pr.errors->head) {
        Cent_update_types(&pr);
    }
    if (!pr.errors->head) {
        Cent_circular_dep(&pr);
    }

    return pr;
}