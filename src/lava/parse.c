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
    Lava_result_init(&result, pd->errors, NULL);

    while (true) {
        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_header) {
            Lava_dom* dom = Lava_parse_header(pd);
            int level = dom->data.LAVA_DOM_HEADER.level;

            if (pd->stack) {
                if (level == 1) {
                    Zinc_error_list_set(pd->errors, &dom->loc, "there are multiple level ones", level);
                    Lava_dom_destroy(dom);
                    free(dom);
                }

                int current_level = pd->stack->dom->data.LAVA_DOM_HEADER.level;
                while (level <= current_level) {
                    Lava_stack_pop(&pd->stack);
                    current_level = pd->stack->dom->data.LAVA_DOM_HEADER.level;
                }

                if (level > current_level + 1) {
                    Zinc_error_list_set(pd->errors, &dom->loc, "level is too high: %d", level);
                } else if (level == current_level + 1) {
                    Zinc_vector_add(&pd->stack->dom->data.LAVA_DOM_HEADER.items, dom, 1);
                    Lava_stack_push(&pd->stack, dom);
                } else {
                    assert(false && "not possible");
                }
            } else {
                Lava_stack_push(&pd->stack, dom);
                result.root = dom;
            }
        } else if (pd->lookahead->kind == Lava_token_kind_text) {
            if (pd->stack) {
                Lava_dom* dom = Lava_parse_text(pd);
                Zinc_vector_add(&pd->stack->dom->data.LAVA_DOM_HEADER.items, dom, 1);
            } else {
                Zinc_error_list_set(pd->errors, &pd->lookahead->loc, "text without header");
            }
        } else if (pd->lookahead->kind == Lava_token_kind_backquote) {
            if (pd->stack) {
                Lava_dom* dom = Lava_parse_backquote(pd);
                Zinc_vector_add(&pd->stack->dom->data.LAVA_DOM_HEADER.items, dom, 1);
            } else {
                Zinc_error_list_set(pd->errors, &pd->lookahead->loc, "backquote without header");
            }
        } else if (pd->lookahead->kind == Lava_token_kind_newline) {
            if (pd->stack) {
                Lava_token* nl = NULL;
                if (!Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, pd->stack->dom)) {
                    assert(false && "not possible");
                }
                Lava_token_destroy(nl);
                free(nl);
            } else {
                Zinc_error_list_set(pd->errors, &pd->lookahead->loc, "newline without header");
            }
        } else if (pd->lookahead->kind == Lava_token_kind_eof) {
            break;
        }
    }

    return result;
}

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
            }
            break;
        }

        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_text) {
            if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
                assert(false && "not possible");
            }
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

    Zinc_string_add_string(&n->data.LAVA_DOM_BACKQUOTE.format, &bq->text);

    Lava_token* txt = NULL;
    Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n);

    if (txt) {
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
    }

    while (pd->lookahead->kind == Lava_token_kind_newline) {
        nl = NULL;
        Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl, n);
        Lava_token_destroy(nl);
        free(nl);

        Lava_lookahead(pd);
        if (pd->lookahead->kind == Lava_token_kind_text) {
            if (!Lava_match(pd, Lava_token_kind_text, "expected text", &txt, n)) {
                assert(false && "not possible");
            }
        }
    }

    Lava_match(pd, Lava_token_kind_backquote, "expected backquote", &bq, n);
    Lava_token_destroy(bq);
    free(bq);

    return n;
}