#include <stdio.h>
#include "ast.h"
#include <stdbool.h>
#include <string.h>
#include "compile.h"
#include "token.h"
#include "compile_data.h"
#include "compile_tools.h"
#include "zinc/buffer.h"
#include <assert.h>

Cob_ast* Cob_parse_union(Cob_compile_data* cd);
Cob_ast* Cob_parse_concat(Cob_compile_data* cd);
Cob_ast* Cob_parse_mod(Cob_compile_data* cd);
Cob_ast* Cob_parse_num(Cob_compile_data* cd);
Cob_ast* Cob_parse_group(Cob_compile_data* cd);
void Cob_parse_seq(Cob_compile_data* cd, Cob_ast* parent);
bool Cob_is_char(enum token_type type);
Cob_ast* Cob_parse_char(Cob_compile_data* cd, bool strict);

Cob_re Cob_compile(Cob_compile_data* cd)
{
    Cob_ast* root = NULL;

    root = Cob_parse_union(cd);

    if (root) {
        root->is_root = true;
    }

    get_lookahead(cd);
    if (cd->lookahead->type != token_eof) {
        error_list_set(cd->el, &cd->lookahead->loc,
        "unhandled token: %d, %c", cd->lookahead->type, cd->lookahead->c);
    }

    Cob_re result = {cd->el, root};
    return result;
}

/* union -> concat union | e' */
/* union' -> '|' concat union' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Cob_ast* Cob_parse_union(Cob_compile_data* cd)
{
    Cob_ast* a = NULL;
    Cob_ast* b = NULL;
    Cob_ast* n = NULL;

    a = Cob_parse_concat(cd);
    if (!a) {
        return a;
    }

    while (true) {
        get_lookahead(cd);

        if (cd->lookahead->type != token_union) {
            break;
        }

        if (!n) {
            Cob_ast_create(&n);
            n->type = Cob_ast_type_union;
            Cob_ast_add(n, a);
        }

        struct token* op = NULL;
        if (!match(cd, token_union, "expected |", &op, n)) {
            n->type = Cob_ast_type_error;
        }
        free(op);

        b = NULL;
        b = Cob_parse_concat(cd);
        if (!b) {
            error_list_set(cd->el, &cd->lookahead->loc, "expected term after union");
            n->type = Cob_ast_type_error;
        } else {
            Cob_ast_add(n, b);
        }
    }

    if (!n) {
        n = a;
    }

    return n;
}

/* concat -> mod concat' | e */
/* concat' -> mod concat' */
/* NOLINTNEXTLINE(misc-no-recursion) */
Cob_ast* Cob_parse_concat(Cob_compile_data* cd)
{
    Cob_ast* a = NULL;
    Cob_ast* b = NULL;
    Cob_ast* n = NULL;

    a = NULL;
    a = Cob_parse_mod(cd);
    if (!a) {
        return a;
    }

    while (true) {
        b = NULL;
        b = Cob_parse_mod(cd);

        if (!b) {
            break;
        }

        if (!n) {
            Cob_ast_create(&n);
            n->type = Cob_ast_type_concat;
            Cob_ast_add(n, a);
        }

        Cob_ast_add(n, b);
    }

    if (!n) {
        n = a;
    }

    return n;
}

bool Cob_is_modifier(enum token_type type)
{
    if (type == token_closure) return true;
    if (type == token_plus) return true;
    if (type == token_open_repeat) return true;
    if (type == token_question) return true;
    return false;
}

/* mod -> group modifier | e */
/* modifier -> { num } | { num , num } | * | + | ? */
/* NOLINTNEXTLINE(misc-no-recursion) */
Cob_ast* Cob_parse_mod(Cob_compile_data* cd)
{
    Cob_ast* a = NULL;
    a = Cob_parse_group(cd);

    if (!a) {
        return a;
    }

    get_lookahead(cd);
    if (!Cob_is_modifier(cd->lookahead->type)) {
        return a;
    }

    Cob_ast* n = NULL;
    Cob_ast_create(&n);
    Cob_ast_add(n, a);

    struct token* mod = NULL;
    if (!match(cd, cd->lookahead->type, "expected modifier", &mod, n)) {
        n->type = Cob_ast_type_error;
    }

    /* parse repeat */
    if (mod->type == token_open_repeat) {
        n->type = Cob_ast_type_repeat;

        Cob_ast* num = Cob_parse_num(cd);
        Cob_ast_add(n, num);

        /* range */
        get_lookahead(cd);
        if (cd->lookahead->type == token_comma) {
            n->type = Cob_ast_type_repeat_range;

            struct token* comma = NULL;
            if (!match(cd, token_comma, "expected a comma", &comma, n)) {
                n->type = Cob_ast_type_error;
            }
            free(comma);

            Cob_ast* num2 = NULL;
            num2 = Cob_parse_num(cd);
            Cob_ast_add(n, num2);
        }

        struct token* ccb = NULL;
        if (!match(cd, token_close_repeat, "expected }", &ccb, n)) {
            n->type = Cob_ast_type_error;
        }
        free(ccb);
    } else if (mod->type == token_closure) {
        n->type = Cob_ast_type_closure;
    } else if (mod->type == token_plus) {
        n->type = Cob_ast_type_positive_closure;
    } else if (mod->type == token_question) {
        n->type = Cob_ast_type_option;
    } else {
        assert(false && "not possible");
    }

    free(mod);

    return n;
}

/* num -> digit num' */
/* num' -> digit num' | e */
Cob_ast* Cob_parse_num(Cob_compile_data* cd)
{
    Cob_ast* n = NULL;
    Cob_ast_create(&n);
    n->type = Cob_ast_type_number;

    struct buffer bf;
    buffer_init(&bf);

    struct token* d = NULL;
    if (!match(cd, token_digit, "expected digit", &d, n)) {
        n->type = Cob_ast_type_error;
        buffer_destroy(&bf);
        return n;
    }

    if (d) {
        buffer_add_char(&bf, d->c[0]);
    }

    free(d);

    get_lookahead(cd);

    while (cd->lookahead->type == token_digit) {
        if (!match(cd, token_digit, "expected digit", &d, n)) {
            assert(false);
        }
        buffer_add_char(&bf, d->c[0]);
        free(d);
        get_lookahead(cd);
    }

    buffer_finish(&bf);
    n->num_value = strtoul(bf.buf, NULL, 10);
    buffer_destroy(&bf);

    return n;
}

/* group -> char | ( union ) | [ seq ] | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Cob_ast* Cob_parse_group(Cob_compile_data* cd)
{
    Cob_ast* n = NULL;

    get_lookahead(cd);

    if (cd->lookahead->type == token_open_paren) {
        Cob_ast_create(&n);
        n->type = Cob_ast_type_group;
        n->is_group = true;
        n->group = ++cd->group_number;

        struct token* opr = NULL;
        if (!match(cd, token_open_paren, "expected left parenthesis", &opr, n)) {
            n->type = Cob_ast_type_error;
            return n;
        }
        free(opr);

        Cob_ast* a = NULL;
        a = Cob_parse_union(cd);

        if (a) {
            Cob_ast_add(n, a);
        }

        struct token* rpr = NULL;
        if (!match(cd, token_close_paren, "expected right parenthesis", &rpr, n)) {
            n->type = Cob_ast_type_error;
        }
        free(rpr);
    } else if (cd->lookahead->type == token_left_square_bracket) {
        Cob_ast_create(&n);

        struct token* lsb = NULL;
        if (!match(cd, token_left_square_bracket, "expected left square bracket", &lsb, n)) {
            assert(false && "not possible");
        }
        free(lsb);

        get_lookahead(cd);
        if (cd->lookahead->type == token_caret) {
            n->type = Cob_ast_type_character_class_opposite;
            struct token* caret = NULL;
            if (!match(cd, token_caret, "expected caret", &caret, n)) {
                assert(false && "not possible");
            }
            free(caret);
        } else {
            n->type = Cob_ast_type_character_class;
        }

        Cob_parse_seq(cd, n);

        struct token* rsb = NULL;
        if (!match(cd, token_right_square_bracket, "expected right square bracket", &rsb, n)) {
            n->type = Cob_ast_type_error;
        }
        free(rsb);
    } else if (Cob_is_char(cd->lookahead->type)) {
        n = Cob_parse_char(cd, false);
        if (n == NULL) {
            assert(false && "not possible");
        }
    }

    return n;
}

/* seq -> char seq' */
/* seq' -> char seq' | e */
void Cob_parse_seq(Cob_compile_data* cd, Cob_ast* parent)
{
    Cob_ast* a = NULL;
    a = Cob_parse_char(cd, true);
    if (!a) {
        error_list_set(cd->el, &cd->lookahead->loc, "expected character in class sequence");
        parent->type = Cob_ast_type_error;
        return;
    }

    Cob_ast_add(parent, a);

    while (true) {
        Cob_ast* b = NULL;
        b = Cob_parse_char(cd, true);
        if (!b) {
            break;
        }

        Cob_ast_add(parent, b);
    }
}

bool Cob_is_char(const enum token_type type)
{
    if (type == token_literal)
        return true;
    if (type == token_wildcard)
        return true;
    if (type == token_caret)
        return true;
    if (type == token_dollar)
        return true;
    if (type == token_backslash)
        return true;

    return false;
}

/* char -> literal | wildcard | begin | end | backslash character */
/* NOLINTNEXTLINE(misc-no-recursion) */
Cob_ast* Cob_parse_char(Cob_compile_data* cd, bool strict)
{
    Cob_ast* n = NULL;

    get_lookahead(cd);
    if (cd->lookahead->type == token_literal) {
        Cob_ast_create(&n);
        n->type = Cob_ast_type_literal;

        struct token* lit = NULL;
        if (!match(cd, token_literal, "expected literal", &lit, n)) {
            assert(false && "not possible");
        }

        if (lit) {
            n->num = lit->num;
            for (int i = 0; i < lit->num; i++) {
                n->c[i] = lit->c[i];
            }
        }
        free(lit);
    } else if (cd->lookahead->type == token_wildcard) {
        Cob_ast_create(&n);
        n->type = Cob_ast_type_wildcard;

        struct token* wc = NULL;
        if (!match(cd, token_wildcard, "expected wildcard", &wc, n)) {
            assert(false && "not possible");
        }

        if (strict) {
            error_list_set(cd->el, &wc->loc, "unexpected wildcard");
            n->type = Cob_ast_type_error;
            free(wc);
            return n;
        }
        free(wc);
    } else if (cd->lookahead->type == token_caret) {
        Cob_ast_create(&n);
        n->type = Cob_ast_type_begin;

        struct token* begin = NULL;
        if (!match(cd, token_caret, "expected begin", &begin, n)) {
            assert(false && "not possible");
        }

        if (strict) {
            error_list_set(cd->el, &begin->loc, "unexpected begin");
            n->type = Cob_ast_type_error;
            free(begin);
            return n;
        }
        free(begin);
    } else if (cd->lookahead->type == token_dollar) {
        Cob_ast_create(&n);
        n->type = Cob_ast_type_end;

        struct token* end = NULL;
        if (!match(cd, token_dollar, "expected end", &end, n)) {
            assert(false && "not possible");
        }

        if (strict) {
            error_list_set(cd->el, &end->loc, "unexpected end");
            n->type = Cob_ast_type_error;
            free(end);
            return n;
        }
        free(end);
    } else if (cd->lookahead->type == token_backslash) {
        bool done = false;
        Cob_ast_create(&n);
        n->type = Cob_ast_type_escape;
        struct token* backslash = NULL;
        if (!match(cd, token_backslash, "expected backslash", &backslash, n)) {
            assert(false && "not possible");
        }
        free(backslash);
        if (backslash) {
            get_lookahead(cd);
            if (cd->lookahead->type == token_literal) {
                if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'w') {
                    n->type = Cob_ast_type_character_type_word;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'W') {
                    n->type = Cob_ast_type_character_type_word_opposite;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'd') {
                    n->type = Cob_ast_type_character_type_digit;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'D') {
                    n->type = Cob_ast_type_character_type_digit_opposite;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 's') {
                    n->type = Cob_ast_type_character_type_space;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'S') {
                    n->type = Cob_ast_type_character_type_space_opposite;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'N') {
                    n->type = Cob_ast_type_character_type_newline_opposite;
                }

                if (n->type != Cob_ast_type_escape) {
                    struct token* lit = NULL;
                    if (!match(cd, token_literal, "expected literal", &lit, n)) {
                        assert(false && "not possible");
                    }
                    free(lit);
                    done = true;
                }
            }

            if (n->type == Cob_ast_type_escape && cd->lookahead->type != token_eof) {
                struct token* ch = NULL;
                if (!match(cd, cd->lookahead->type, "expected character", &ch, n)) {
                    assert(false && "not possible");
                }
                n->num = ch->num;
                for (int i = 0; i < n->num; i++) {
                    n->c[i] = ch->c[i];
                }
                free(ch);
                done = true;
            }

            if (!done) {
                error_list_set(cd->el, &cd->lookahead->loc, "missing character in escape sequence");
                n->type = Cob_ast_type_error;
            }
        }
    }

    /* if the character is part of a range */
    if (n && strict) {
        get_lookahead(cd);
        if (cd->lookahead->type == token_dash) {
            struct token* dash = NULL;
            if (!match(cd, token_dash, "expected range", &dash, n)) {
                assert(false && "not possible");
            }
            free(dash);

            Cob_ast* a = n;
            Cob_ast_create(&n);
            n->type = Cob_ast_type_character_range;
            Cob_ast_add(n, a);
            if (a->num != 1) {
                error_list_set(cd->el, &a->loc, "character range must use ascii characters");
                n->type = Cob_ast_type_error;
            }

            Cob_ast* b = NULL;
            b = Cob_parse_char(cd, true);
            if (b) {
                Cob_ast_add(n, b);
                if (b->num != 1) {
                    error_list_set(cd->el, &b->loc, "character range must use ascii characters");
                    n->type = Cob_ast_type_error;
                }
            } else {
                get_lookahead(cd);
                error_list_set(cd->el, &cd->lookahead->loc, "expected end character in character range");
                n->type = Cob_ast_type_error;
            }
        }
    }

    return n;
}