#include "dom.h"
#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include "zinc/input_unicode_string.h"
#include <string.h>

Lava_result Lava_parse(Lava_parse_data* pd);
Lava_dom* Lava_parse_header(Lava_parse_data* pd);
Lava_dom* Lava_parse_text(Lava_parse_data* pd);
Lava_dom* Lava_parse_backquote(Lava_parse_data* pd);

Lava_result Lava_parse_str(char* s)
{
    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(char));
    Zinc_vector_add(v, s, strlen(s));

    Zinc_error_list* errors = NULL;
    Zinc_error_list_create(&errors);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, v);

    Lava_lex_data* ld = NULL;
    Lava_lex_data_create(&ld, errors, input, input->vtable);

    Lava_parse_data* pd = NULL;
    Lava_parse_data_create(&pd, ld, errors);

    Lava_result pr = Lava_parse(pd);

    Zinc_input_unicode_string_destroy(input);
    free(input);

    free(ld);

    free(pd);

    return pr;
}

Lava_result Lava_parse(Lava_parse_data* pd)
{
    Lava_result result;

    Lava_dom* root = Lava_parse_header(pd);
    if (root->data.LAVA_DOM_HEADER.level != 1) {
        Zinc_error_list_set(pd->errors, &root->loc, "expected top level header");
    }

    Lava_result_init(&result, pd->errors, root);

    return result;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Lava_dom* Lava_parse_header(Lava_parse_data* pd)
{
    Lava_dom* n = NULL;
    Lava_dom_create(&n, LAVA_DOM_HEADER);

    Lava_token* hdr = NULL;
    Lava_match(pd, Lava_token_kind_header, "expected header", &hdr, n);

    if (hdr) {
        n->data.LAVA_DOM_HEADER.level = (int)hdr->text.size;
        Lava_token_destroy(hdr);
        free(hdr);
    }

    Lava_token* title = NULL;
    Lava_match(pd, Lava_token_kind_text, "expected header title", &title, n);

    if (title) {
        Zinc_string_add_string(&n->data.LAVA_DOM_HEADER.title, &title->text);
        Lava_token_destroy(title);
        free(title);
    }

    Lava_token* nl = NULL;
    Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n);
    Lava_token_destroy(nl);
    free(nl);

    while (true) {
        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_header) {
            if (pd->lookahead->text.size <= n->data.LAVA_DOM_HEADER.level) {
                break;
            }

            Lava_dom* dom = Lava_parse_header(pd);
            int level = dom->data.LAVA_DOM_HEADER.level;
            int current_level = n->data.LAVA_DOM_HEADER.level;
            if (level > current_level + 1) {
                Zinc_error_list_set(pd->errors, &dom->loc, "level is too high: %d", level);
            } else if (level == current_level + 1) {
                Zinc_vector_add(&n->data.LAVA_DOM_HEADER.items, dom, 1);
            } else {
                assert(false && "not possible");
            }
        } else if (pd->lookahead->kind == Lava_token_kind_text) {
            Lava_dom* dom = Lava_parse_text(pd);
            Zinc_vector_add(&n->data.LAVA_DOM_HEADER.items, dom, 1);
        } else if (pd->lookahead->kind == Lava_token_kind_backquote) {
            Lava_dom* dom = Lava_parse_backquote(pd);
            Zinc_vector_add(&n->data.LAVA_DOM_HEADER.items, dom, 1);
        } else if (pd->lookahead->kind == Lava_token_kind_newline) {
            Lava_token* nl = NULL;
            if (!Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n)) {
                assert(false && "not possible");
            }
            Lava_token_destroy(nl);
            free(nl);
        } else if (pd->lookahead->kind == Lava_token_kind_eof) {
            break;
        }
    }

    return n;
}

Lava_dom* Lava_parse_text(Lava_parse_data* pd)
{
    Lava_dom* n = NULL;
    Lava_dom_create(&n, LAVA_DOM_TEXT);

    Lava_token* txt = NULL;
    if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
        assert(false && "not possible");
    }

    Zinc_string_add_string(&n->data.LAVA_DOM_TEXT, &txt->text);
    Lava_token_destroy(txt);
    free(txt);

    Lava_lookahead(pd);
    while (pd->lookahead->kind == Lava_token_kind_newline) {
        Lava_token* nl = NULL;
        if (!Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n)) {
            assert(false && "not possible");
        }
        Lava_token_destroy(nl);
        free(nl);

        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_newline) {
            while (pd->lookahead->kind == Lava_token_kind_newline) {
                if (!Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n)) {
                    assert(false && "not possible");
                }
                Lava_lookahead(pd);
            }
            break;
        }

        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_text) {
            if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
                assert(false && "not possible");
            }
            Zinc_string_add_format(&n->data.LAVA_DOM_TEXT, " %bf", &txt->text);
            Lava_token_destroy(txt);
            free(txt);
        }

        Lava_lookahead(pd);
    }

    return n;
}

Lava_dom* Lava_parse_backquote(Lava_parse_data* pd)
{
    Lava_dom* n = NULL;
    Lava_dom_create(&n, LAVA_DOM_BACKQUOTE);

    Lava_token* bq = NULL;
    if (!Lava_match(pd, Lava_token_kind_backquote, "expected backquote", &bq, n)) {
        assert(false && "not possible");
    }

    Lava_token* txt = NULL;

    Lava_lookahead(pd);
    if (pd->lookahead->kind == Lava_token_kind_text) {
        if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
            assert(false && "not possible");
        }
        Zinc_string_add_string(&n->data.LAVA_DOM_BACKQUOTE.format, &txt->text);
        Lava_token_destroy(txt);
        free(txt);
    }

    Lava_token* nl = NULL;
    Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n);
    Lava_token_destroy(nl);
    free(nl);

    Lava_lookahead(pd);
    if (pd->lookahead->kind == Lava_token_kind_text) {
        if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
            assert(false && "not possible");
        }
        if (txt) {
            Zinc_string_add_string(&n->data.LAVA_DOM_BACKQUOTE.text, &txt->text);
            Lava_token_destroy(txt);
            free(txt);
        }
    }

    Lava_lookahead(pd);
    while (pd->lookahead->kind == Lava_token_kind_newline) {
        nl = NULL;
        Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n);
        Zinc_string_add_char(&n->data.LAVA_DOM_BACKQUOTE.text, '\n');
        Lava_token_destroy(nl);
        free(nl);

        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_text) {
            if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
                assert(false && "not possible");
            }
            Zinc_string_add_string(&n->data.LAVA_DOM_BACKQUOTE.text, &txt->text);
        }
        Lava_lookahead(pd);
    }

    Lava_match(pd, Lava_token_kind_backquote, "expected backquote", &bq, n);
    Lava_token_destroy(bq);
    free(bq);

    return n;
}