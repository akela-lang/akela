#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include <akela/parse_tools.h>
#include "base.h"
#include "token.h"
#include "update_types.h"
#include "parse_stmts.h"
#include "update_values.h"
#include "check_types.h"

Cent_parse_result Cent_parse(Cent_parse_data* pd)
{
    pd->top = pd->base;
    Cent_ast* root = Cent_parse_stmts(pd);
    if (pd->lookahead->type != Cent_token_eof) {
        Zinc_error_list_set(
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
    Cent_parse_result_init(&pr, pd->errors);
    pr.base = pd->top;
    pr.root = root;

    if (!pr.errors->head) {
        Cent_parse_process_tasks(pd, &pr);
    }

    if (!pr.errors->head) {
        Cent_update_values(&pr);
    }

    return pr;
}