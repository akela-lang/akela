#include <stdio.h>
#include "Ast_node.h"
#include <stdbool.h>
#include <string.h>
#include "compile.h"
#include "token.h"
#include "compile_data.h"
#include "compile_tools.h"
#include "zinc/buffer.h"
#include <assert.h>

Ast_node* parse_union(struct compile_data* cd);
Ast_node* parse_concat(struct compile_data* cd);
Ast_node* parse_mod(struct compile_data* cd);
Ast_node* parse_num(struct compile_data* cd);
Ast_node* parse_group(struct compile_data* cd);
void parse_seq(struct compile_data* cd, Ast_node* parent);
bool is_char(enum token_type type);
Ast_node* parse_char(struct compile_data* cd, bool strict);

bool compile(struct compile_data* cd, Ast_node** root)
{
    bool valid = true;

    *root = parse_union(cd);
    if (*root && (*root)->type == Ast_type_error) {
        valid = false;
    }

    /* set in case it was not set because there were no child groups */
    if (*root) {
        Ast_node_set_root(*root);
    }

    get_lookahead(cd);
    if (cd->lookahead->type != token_eof) {
        error_list_set(cd->el, &cd->lookahead->loc,
        "unhandled token: %d, %c", cd->lookahead->type, cd->lookahead->c);
    }
    return valid;
}

/* union -> concat union | e' */
/* union' -> '|' concat union' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ast_node* parse_union(struct compile_data* cd)
{
    Ast_node* a = NULL;
    Ast_node* b = NULL;
    Ast_node* n = NULL;

    a = parse_concat(cd);
    if (!a) {
        return a;
    }

    while (true) {
        get_lookahead(cd);

        if (cd->lookahead->type != token_union) {
            break;
        }

        if (!n) {
            Ast_node_create(&n);
            n->type = Ast_type_union;
            Ast_node_add(n, a);
        }

        struct token* op = NULL;
        if (!match(cd, token_union, "expected |", &op, n)) {
            n->type = Ast_type_error;
        }
        free(op);

        b = NULL;
        b = parse_concat(cd);
        if (!b) {
            error_list_set(cd->el, &cd->lookahead->loc, "expected term after union");
            n->type = Ast_type_error;
        } else {
            Ast_node_add(n, b);
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
Ast_node* parse_concat(struct compile_data* cd)
{
    Ast_node* a = NULL;
    Ast_node* b = NULL;
    Ast_node* n = NULL;

    a = NULL;
    a = parse_mod(cd);
    if (!a) {
        return a;
    }

    while (true) {
        b = NULL;
        b = parse_mod(cd);

        if (!b) {
            break;
        }

        if (!n) {
            Ast_node_create(&n);
            n->type = Ast_type_concat;
            Ast_node_add(n, a);
        }

        Ast_node_add(n, b);
    }

    if (!n) {
        n = a;
    }

    return n;
}

bool is_modifier(enum token_type type)
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
Ast_node* parse_mod(struct compile_data* cd)
{
    Ast_node* a = NULL;
    a = parse_group(cd);

    if (!a) {
        return a;
    }

    get_lookahead(cd);
    if (!is_modifier(cd->lookahead->type)) {
        return a;
    }

    Ast_node* n = NULL;
    Ast_node_create(&n);
    Ast_node_add(n, a);

    struct token* mod = NULL;
    if (!match(cd, cd->lookahead->type, "expected modifier", &mod, n)) {
        n->type = Ast_type_error;
    }

    /* parse repeat */
    if (mod->type == token_open_repeat) {
        n->type = Ast_type_repeat;

        Ast_node* num = parse_num(cd);
        Ast_node_add(n, num);

        /* range */
        get_lookahead(cd);
        if (cd->lookahead->type == token_comma) {
            n->type = Ast_type_repeat_range;

            struct token* comma = NULL;
            if (!match(cd, token_comma, "expected a comma", &comma, n)) {
                n->type = Ast_type_error;
            }
            free(comma);

            Ast_node* num2 = NULL;
            num2 = parse_num(cd);
            Ast_node_add(n, num2);
        }

        struct token* ccb = NULL;
        if (!match(cd, token_close_repeat, "expected }", &ccb, n)) {
            n->type = Ast_type_error;
        }
        free(ccb);
    } else if (mod->type == token_closure) {
        n->type = Ast_type_closure;
    } else if (mod->type == token_plus) {
        n->type = Ast_type_positive_closure;
    } else if (mod->type == token_question) {
        n->type = Ast_type_option;
    } else {
        assert(false && "not possible");
    }

    free(mod);

    return n;
}

/* num -> digit num' */
/* num' -> digit num' | e */
Ast_node* parse_num(struct compile_data* cd)
{
    Ast_node* n = NULL;
    Ast_node_create(&n);
    n->type = Ast_type_number;

    struct buffer bf;
    buffer_init(&bf);

    struct token* d = NULL;
    if (!match(cd, token_digit, "expected digit", &d, n)) {
        n->type = Ast_type_error;
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
Ast_node* parse_group(struct compile_data* cd)
{
    Ast_node* n = NULL;

    get_lookahead(cd);

    if (cd->lookahead->type == token_open_paren) {
        Ast_node_create(&n);
        n->type = Ast_type_group;

        struct token* opr = NULL;
        if (!match(cd, token_open_paren, "expected left parenthesis", &opr, n)) {
            n->type = Ast_type_error;
            return n;
        }
        free(opr);

        Ast_node* a = NULL;
        a = parse_union(cd);

        if (a) {
            Ast_node_add(n, a);
        }

        struct token* rpr = NULL;
        if (!match(cd, token_close_paren, "expected right parenthesis", &rpr, n)) {
            n->type = Ast_type_error;
        }
        free(rpr);
    } else if (cd->lookahead->type == token_left_square_bracket) {
        Ast_node_create(&n);

        struct token* lsb = NULL;
        if (!match(cd, token_left_square_bracket, "expected left square bracket", &lsb, n)) {
            assert(false && "not possible");
        }

        get_lookahead(cd);
        if (cd->lookahead->type == token_caret) {
            n->type = Ast_type_character_class_opposite;
            if (!match(cd, token_caret, "expected caret", &lsb, n)) {
                assert(false && "not possible");
            }
        } else {
            n->type = Ast_type_character_class;
        }

        free(lsb);

        parse_seq(cd, n);

        struct token* rsb = NULL;
        if (!match(cd, token_right_square_bracket, "expected right square bracket", &rsb, n)) {
            n->type = Ast_type_error;
        }
        free(rsb);
    } else if (is_char(cd->lookahead->type)) {
        n = parse_char(cd, false);
        if (n == NULL) {
            assert(false && "not possible");
        }
    }

    return n;
}

/* seq -> char seq' */
/* seq' -> char seq' | e */
void parse_seq(struct compile_data* cd, Ast_node* parent)
{
    Ast_node* a = NULL;
    a = parse_char(cd, true);
    if (!a) {
        error_list_set(cd->el, &cd->lookahead->loc, "expected character in class sequence");
        parent->type = Ast_type_error;
        return;
    }

    Ast_node_add(parent, a);

    while (true) {
        Ast_node* b = NULL;
        b = parse_char(cd, true);
        if (!b) {
            break;
        }

        Ast_node_add(parent, b);
    }
}

bool is_char(const enum token_type type)
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
Ast_node* parse_char(struct compile_data* cd, bool strict)
{
    Ast_node* n = NULL;

    get_lookahead(cd);
    if (cd->lookahead->type == token_literal) {
        Ast_node_create(&n);
        n->type = Ast_type_literal;

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
        Ast_node_create(&n);
        n->type = Ast_type_wildcard;

        struct token* wc = NULL;
        if (!match(cd, token_wildcard, "expected wildcard", &wc, n)) {
            assert(false && "not possible");
        }

        if (strict) {
            error_list_set(cd->el, &wc->loc, "unexpected wildcard");
            n->type = Ast_type_error;
            return n;
        }
        free(wc);
    } else if (cd->lookahead->type == token_caret) {
        Ast_node_create(&n);
        n->type = Ast_type_begin;

        struct token* begin = NULL;
        if (!match(cd, token_caret, "expected begin", &begin, n)) {
            assert(false && "not possible");
        }

        if (strict) {
            error_list_set(cd->el, &begin->loc, "unexpected begin");
            n->type = Ast_type_error;
            return n;
        }
        free(begin);
    } else if (cd->lookahead->type == token_dollar) {
        Ast_node_create(&n);
        n->type = Ast_type_end;

        struct token* end = NULL;
        if (!match(cd, token_dollar, "expected end", &end, n)) {
            assert(false && "not possible");
        }

        if (strict) {
            error_list_set(cd->el, &end->loc, "unexpected end");
            n->type = Ast_type_error;
            return n;
        }
        free(end);
    } else if (cd->lookahead->type == token_backslash) {
        bool done = false;
        Ast_node_create(&n);
        n->type = Ast_type_escape;
        struct token* backslash = NULL;
        if (!match(cd, token_backslash, "expected backslash", &backslash, n)) {
            assert(false && "not possible");
        }
        free(backslash);
        if (backslash) {
            get_lookahead(cd);
            if (cd->lookahead->type == token_literal) {
                if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'w') {
                    n->type = Ast_type_character_type_word;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'W') {
                    n->type = Ast_type_character_type_word_opposite;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'd') {
                    n->type = Ast_type_character_type_digit;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'D') {
                    n->type = Ast_type_character_type_digit_opposite;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 's') {
                    n->type = Ast_type_character_type_space;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'S') {
                    n->type = Ast_type_character_type_space_opposite;
                } else if (cd->lookahead->num == 1 && cd->lookahead->c[0] == 'N') {
                    n->type = Ast_type_character_type_newline_opposite;
                }

                if (n->type != Ast_type_escape) {
                    struct token* lit = NULL;
                    if (!match(cd, token_literal, "expected literal", &lit, n)) {
                        assert(false && "not possible");
                    }
                    free(lit);
                    done = true;
                }
            }

            if (n->type == Ast_type_escape && cd->lookahead->type != token_eof) {
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
                n->type = Ast_type_error;
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

            Ast_node* a = n;
            Ast_node_create(&n);
            n->type = Ast_type_character_range;
            Ast_node_add(n, a);
            if (a->num != 1) {
                error_list_set(cd->el, &a->loc, "character range must use ascii characters");
                n->type = Ast_type_error;
            }

            Ast_node* b = NULL;
            b = parse_char(cd, true);
            if (b) {
                Ast_node_add(n, b);
                if (b->num != 1) {
                    error_list_set(cd->el, &b->loc, "character range must use ascii characters");
                    n->type = Ast_type_error;
                }
            } else {
                get_lookahead(cd);
                error_list_set(cd->el, &cd->lookahead->loc, "expected end character in character range");
                n->type = Ast_type_error;
            }
        }
    }

    return n;
}