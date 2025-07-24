#include "dom.h"
#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include "zinc/input_unicode_string.h"
#include <string.h>
#include <zinc/input_unicode_file.h>

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

Lava_result Lava_parse_file(FILE* fp)
{
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    Zinc_error_list* errors = NULL;
    Zinc_error_list_create(&errors);

    Lava_lex_data* ld = NULL;
    Lava_lex_data_create(&ld, errors, input, input->vtable);

    Lava_parse_data* pd = NULL;
    Lava_parse_data_create(&pd, ld, errors);

    Lava_result pr = Lava_parse(pd);

    Zinc_input_unicode_file_destroy(input);
    free(input);
    free(ld);
    free(pd);

    return pr;
}

Lava_result Lava_parse(Lava_parse_data* pd)
{
    Lava_result result;

    Lava_dom* root = Lava_parse_header(pd);
    if (root->data.header.level != 1) {
        Zinc_error_list_set(pd->errors, &root->loc, "expected top level header");
    }

    if (pd->lookahead) {
        if (pd->lookahead->kind != Lava_token_kind_eof) {
            Zinc_error_list_set(
                pd->errors,
                &pd->lookahead->loc,
                "could not process token: %d-%s",
                pd->lookahead->kind,
                Lava_token_kind_name(pd->lookahead->kind));
        }
        Lava_token_destroy(pd->lookahead);
        free(pd->lookahead);
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
        n->data.header.level = (int)hdr->text.size;
        Lava_token_destroy(hdr);
        free(hdr);
    }

    Lava_token* title = NULL;
    Lava_match(pd, Lava_token_kind_text, "expected header title", &title, n);

    if (title) {
        Zinc_string_add_string(&n->data.header.title, &title->text);
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
            if (pd->lookahead->text.size <= n->data.header.level) {
                break;
            }

            Lava_dom* dom = Lava_parse_header(pd);
            int level = dom->data.header.level;
            int current_level = n->data.header.level;
            if (level > current_level + 1) {
                Zinc_error_list_set(pd->errors, &dom->loc, "level is too high: %d", level);
            } else if (level == current_level + 1) {
                Lava_dom_add(n, dom);
            } else {
                assert(false && "not possible");
            }
        } else if (pd->lookahead->kind == Lava_token_kind_text) {
            Lava_dom* dom = Lava_parse_text(pd);
            Lava_dom_add(n, dom);
        } else if (pd->lookahead->kind == Lava_token_kind_backquote) {
            Lava_dom* dom = Lava_parse_backquote(pd);
            Lava_dom_add(n, dom);
        } else if (pd->lookahead->kind == Lava_token_kind_newline) {
            nl = NULL;
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

    Zinc_string_add_string(&n->data.text, &txt->text);
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
                Lava_token_destroy(nl);
                free(nl);
                Lava_lookahead(pd);
            }
            break;
        }

        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_text) {
            if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
                assert(false && "not possible");
            }
            Zinc_string_add_format(&n->data.text, " %bf", &txt->text);
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
    Lava_token_destroy(bq);
    free(bq);

    Lava_token* txt = NULL;

    Lava_lookahead(pd);
    if (pd->lookahead->kind == Lava_token_kind_text) {
        if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
            assert(false && "not possible");
        }
        Zinc_string_add_string(&n->data.backquote.format, &txt->text);
        Lava_token_destroy(txt);
        free(txt);
    }

    Lava_token* nl = NULL;
    Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n);
    Lava_token_destroy(nl);
    free(nl);

    Lava_lookahead(pd);
    n->data.backquote.bounds.loc = pd->lookahead->loc;

    while (true) {
        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_eof) {
            break;
        }
        if (pd->lookahead->kind == Lava_token_kind_backquote) {
            break;
        }

        if (pd->lookahead->kind == Lava_token_kind_newline) {
            if (!Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n)) {
                assert(false && "not possible");
            }
            Zinc_string_add_str(&n->data.backquote.text, "\n");
            Lava_token_destroy(nl);
            free(nl);
        } else if (pd->lookahead->kind == Lava_token_kind_header) {
            Lava_token* header = NULL;
            if (!Lava_match(pd, Lava_token_kind_header, "expected header", &header, n)) {
                assert(false && "not possible");
            }
            Zinc_string_add_string(&n->data.backquote.text, &header->text);
            Lava_token_destroy(header);
            free(header);
        } else if (pd->lookahead->kind == Lava_token_kind_text) {
            if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
                assert(false && "not possible");
            }
            Zinc_string_add_string(&n->data.backquote.text, &txt->text);
            Lava_token_destroy(txt);
            free(txt);
        } else {
            assert(false && "unhandled token kind");
        }
    }

    Lava_lookahead(pd);
    n->data.backquote.bounds.end = pd->lookahead->loc.start;
    Lava_match(pd, Lava_token_kind_backquote, "expected backquote", &bq, n);
    Lava_token_destroy(bq);
    free(bq);

    Lava_lookahead(pd);
    if (pd->lookahead->kind == Lava_token_kind_newline) {
        if (!Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n)) {
            assert(false && "not possible");
        }
        Lava_token_destroy(nl);
        free(nl);
    }

    return n;
}