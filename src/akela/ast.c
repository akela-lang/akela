#define AKELA_AST_C

#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/memory.h"
#include "zinc/list.h"
#include "type.h"

void Ake_AstCreate(Ake_Ast** n)
{
    Zinc_malloc_safe((void**)n, sizeof(Ake_Ast));
    Ake_AstInit(*n);
}

void Ake_AstInit(Ake_Ast* n)
{
    n->kind = AKE_AST_NONE;
    Zinc_string_init(&n->struct_value);
    n->type = NULL;
    Zinc_location_init(&n->loc);
    n->next = NULL;
    n->prev = NULL;
    n->head = NULL;
    n->tail = NULL;
    n->parent = NULL;
    n->has_error = false;
    n->is_set = false;
    Ake_token_list_init(&n->token_list);
}

void Ake_AstSet(Ake_Ast* n, Ake_AstKind kind)
{
    assert(!n->is_set);
    n->kind = kind;
    switch (kind) {
        case AKE_AST_ID:
            Zinc_string_init(&n->data.id.value);
            n->is_set = true;
            break;
        case AKE_AST_SIGN:
            n->data.sign.op = NULL;
            n->data.sign.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_NUMBER:
            Zinc_string_init(&n->data.number.value);
            n->is_set = true;
            break;
        case AKE_AST_STRING:
            Zinc_string_init(&n->data.string.value);
            n->is_set = true;
            break;
        case AKE_AST_ASSIGN:
            n->data.assign.left = NULL;
            n->data.assign.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_PLUS:
            n->data.plus.left = NULL;
            n->data.plus.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_MINUS:
            n->data.minus.left = NULL;
            n->data.minus.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_MULT:
            n->data.mult.left = NULL;
            n->data.mult.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_DIVIDE:
            n->data.divide.left = NULL;
            n->data.divide.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_STMTS:
            Ake_AstListInit(&n->data.stmts.list, n);
            n->is_set = true;
            break;
        case AKE_AST_FUNCTION:
            n->data.function.proto = NULL;
            n->data.function.body = NULL;
            n->is_set = true;
            break;
        case AKE_AST_DSEQ:
            Ake_AstListInit(&n->data.dseq.list, n);
            n->is_set = true;
            break;
        case AKE_AST_DRET:
            n->data.dret.node = NULL;
            n->is_set = true;
            break;
        case AKE_AST_CALL:
            n->data.call.func = NULL;
            Ake_AstListInit(&n->data.call.args, n);
            n->is_set = true;
            break;
        case AKE_AST_IF:
            Ake_AstListInit(&n->data._if_.branches, n);
            n->is_set = true;
            break;
        case AKE_AST_COND_BRANCH:
            n->data.cond_branch.cond = NULL;
            n->data.cond_branch.body = NULL;
            n->is_set = true;
            break;
        case AKE_AST_DEFAULT_BRANCH:
            n->data.default_branch.body = NULL;
            n->is_set = true;
            break;
        case AKE_AST_EQUALITY:
            n->data.equality.left = NULL;
            n->data.equality.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_NOT_EQUAL:
            n->data.not_equal.left = NULL;
            n->data.not_equal.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_LESS_THAN:
            n->data.less_than.left = NULL;
            n->data.less_than.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_LESS_THAN_OR_EQUAL:
            n->data.less_than_or_equal.left = NULL;
            n->data.less_than_or_equal.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_GREATER_THAN:
            n->data.greater_than.left = NULL;
            n->data.greater_than.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_GREATER_THAN_OR_EQUAL:
            n->data.greater_than_or_equal.left = NULL;
            n->data.greater_than_or_equal.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_NOT:
            n->data._not_.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_AND:
            n->data._and_.left = NULL;
            n->data._and_.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_OR:
            n->data._or_.left = NULL;
            n->data._or_.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_WHILE:
            n->data._while_.cond = NULL;
            n->data._while_.body = NULL;
            n->is_set = true;
            break;
        case AKE_AST_FOR_RANGE:
            n->data.for_range.dec = NULL;
            n->data.for_range.start = NULL;
            n->data.for_range.end = NULL;
            n->data.for_range.body = NULL;
            n->is_set = true;
            break;
        case AKE_AST_FOR_ITERATION:
            n->data.for_iteration.dec = NULL;
            n->data.for_iteration.iterator = NULL;
            n->data.for_iteration.body = NULL;
            n->is_set = true;
            break;
        case AKE_AST_DECLARATION:
            n->data.declaration.id_node = NULL;
            n->data.declaration.type_node = NULL;
            n->is_set = true;
            break;
        case AKE_AST_ARRAY_LITERAL:
            Ake_AstListInit(&n->data.array_literal.list, n);
            n->is_set = true;
        case AKE_AST_ARRAY_SUBSCRIPT:
            n->data.array_subscript.array = NULL;
            n->data.array_subscript.index = NULL;
            n->is_set = true;
            break;
        case AKE_AST_BOOLEAN:
            Zinc_string_init(&n->data.boolean.value);
            n->is_set = true;
            break;
        case AKE_AST_PARENTHESIS:
            n->data.parenthesis.expr = NULL;
            n->is_set = true;
            break;
        case AKE_AST_TYPE:
            n->is_set = true;
            break;
        case AKE_AST_POWER:
            n->data.power.left = NULL;
            n->data.power.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_DOT:
            n->data.dot.left = NULL;
            n->data.dot.right = NULL;
            n->is_set = true;
            break;
        case AKE_AST_STRUCT:
            Ake_AstListInit(&n->data._struct_.list, n);
            n->is_set = true;
            break;
        case AKE_AST_RETURN:
            n->data._return_.expr = NULL;
            n->is_set = true;
            break;
        case AKE_AST_PROTOTYPE:
            n->data.prototype.id = NULL;
            n->data.prototype.dseq = NULL;
            n->data.prototype.ret = NULL;
            n->is_set = true;
            break;
        default:
            break;
    }
}

void Ake_AstValidate(Ake_Ast* n)
{
    switch(n->kind) {
        case AKE_AST_ID:
        case AKE_AST_SIGN:
        case AKE_AST_NUMBER:
        case AKE_AST_STRING:
        case AKE_AST_ASSIGN:
        case AKE_AST_PLUS:
        case AKE_AST_MINUS:
        case AKE_AST_MULT:
        case AKE_AST_DIVIDE:
        case AKE_AST_STMTS:
        case AKE_AST_FUNCTION:
        case AKE_AST_DSEQ:
        case AKE_AST_DRET:
        case AKE_AST_CALL:
        case AKE_AST_IF:
        case AKE_AST_COND_BRANCH:
        case AKE_AST_DEFAULT_BRANCH:
        case AKE_AST_EQUALITY:
        case AKE_AST_NOT_EQUAL:
        case AKE_AST_LESS_THAN:
        case AKE_AST_LESS_THAN_OR_EQUAL:
        case AKE_AST_GREATER_THAN:
        case AKE_AST_NOT:
        case AKE_AST_AND:
        case AKE_AST_OR:
        case AKE_AST_WHILE:
        case AKE_AST_FOR_RANGE:
        case AKE_AST_FOR_ITERATION:
        case AKE_AST_DECLARATION:
        case AKE_AST_ARRAY_LITERAL:
        case AKE_AST_ARRAY_SUBSCRIPT:
        case AKE_AST_BOOLEAN:
        case AKE_AST_PARENTHESIS:
        case AKE_AST_TYPE:
        case AKE_AST_POWER:
        case AKE_AST_DOT:
        case AKE_AST_STRUCT:
        case AKE_AST_RETURN:
        case AKE_AST_PROTOTYPE:
            assert(n->is_set);
            break;
        default:
            break;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_AstDestroy(Ake_Ast* n)
{
    if (n) {
        Ake_AstValidate(n);

        Ake_Ast* p = NULL;
        switch(n->kind) {
            case AKE_AST_ID:
                Zinc_string_destroy(&n->data.id.value);
                break;
            case AKE_AST_SIGN:
                Ake_AstDestroy(n->data.sign.op);
                Ake_AstDestroy(n->data.sign.right);
                break;
            case AKE_AST_NUMBER:
                Zinc_string_destroy(&n->data.number.value);
                break;
            case AKE_AST_STRING:
                Zinc_string_destroy(&n->data.string.value);
                break;
            case AKE_AST_ASSIGN:
                Ake_AstDestroy(n->data.assign.left);
                Ake_AstDestroy(n->data.assign.right);
                break;
            case AKE_AST_PLUS:
                Ake_AstDestroy(n->data.plus.left);
                Ake_AstDestroy(n->data.plus.right);
                break;
            case AKE_AST_MINUS:
                Ake_AstDestroy(n->data.minus.left);
                Ake_AstDestroy(n->data.minus.right);
                break;
            case AKE_AST_MULT:
                Ake_AstDestroy(n->data.mult.left);
                Ake_AstDestroy(n->data.mult.right);
                break;
            case AKE_AST_DIVIDE:
                Ake_AstDestroy(n->data.divide.left);
                Ake_AstDestroy(n->data.divide.right);
                break;
            case AKE_AST_STMTS:
                Ake_AstListDestroy(&n->data.stmts.list);
                break;
            case AKE_AST_FUNCTION:
                Ake_AstDestroy(n->data.function.proto);
                Ake_AstDestroy(n->data.function.body);
                break;
            case AKE_AST_DSEQ:
                Ake_AstListDestroy(&n->data.dseq.list);
                break;
            case AKE_AST_DRET:
                Ake_AstDestroy(n->data.dret.node);
                break;
            case AKE_AST_CALL:
                Ake_AstDestroy(n->data.call.func);
                Ake_AstListDestroy(&n->data.call.args);
                break;
            case AKE_AST_IF:
                Ake_AstListDestroy(&n->data._if_.branches);
                break;
            case AKE_AST_COND_BRANCH:
                Ake_AstDestroy(n->data.cond_branch.cond);
                Ake_AstDestroy(n->data.cond_branch.body);
                break;
            case AKE_AST_DEFAULT_BRANCH:
                Ake_AstDestroy(n->data.default_branch.body);
                break;
            case AKE_AST_EQUALITY:
                Ake_AstDestroy(n->data.equality.left);
                Ake_AstDestroy(n->data.equality.right);
                break;
            case AKE_AST_NOT_EQUAL:
                Ake_AstDestroy(n->data.not_equal.left);
                Ake_AstDestroy(n->data.not_equal.right);
                break;
            case AKE_AST_LESS_THAN:
                Ake_AstDestroy(n->data.less_than.left);
                Ake_AstDestroy(n->data.less_than.right);
                break;
            case AKE_AST_LESS_THAN_OR_EQUAL:
                Ake_AstDestroy(n->data.less_than_or_equal.left);
                Ake_AstDestroy(n->data.less_than_or_equal.right);
                break;
            case AKE_AST_GREATER_THAN:
                Ake_AstDestroy(n->data.greater_than.left);
                Ake_AstDestroy(n->data.greater_than.right);
                break;
            case AKE_AST_GREATER_THAN_OR_EQUAL:
                Ake_AstDestroy(n->data.greater_than_or_equal.left);
                Ake_AstDestroy(n->data.greater_than_or_equal.right);
                break;
            case AKE_AST_NOT:
                Ake_AstDestroy(n->data._not_.right);
                break;
            case AKE_AST_AND:
                Ake_AstDestroy(n->data._and_.left);
                Ake_AstDestroy(n->data._and_.right);
                break;
            case AKE_AST_OR:
                Ake_AstDestroy(n->data._or_.left);
                Ake_AstDestroy(n->data._or_.right);
                break;
            case AKE_AST_WHILE:
                Ake_AstDestroy(n->data._while_.cond);
                Ake_AstDestroy(n->data._while_.body);
                break;
            case AKE_AST_FOR_RANGE:
                Ake_AstDestroy(n->data.for_range.dec);
                Ake_AstDestroy(n->data.for_range.start);
                Ake_AstDestroy(n->data.for_range.end);
                Ake_AstDestroy(n->data.for_range.body);
                break;
            case AKE_AST_FOR_ITERATION:
                Ake_AstDestroy(n->data.for_iteration.dec);
                Ake_AstDestroy(n->data.for_iteration.iterator);
                Ake_AstDestroy(n->data.for_iteration.body);
                break;
            case AKE_AST_DECLARATION:
                Ake_AstDestroy(n->data.declaration.id_node);
                Ake_AstDestroy(n->data.declaration.type_node);
                break;
            case AKE_AST_ARRAY_LITERAL:
                Ake_AstListDestroy(&n->data.array_literal.list);
                break;
            case AKE_AST_ARRAY_SUBSCRIPT:
                Ake_AstDestroy(n->data.array_subscript.array);
                Ake_AstDestroy(n->data.array_subscript.index);
                break;
            case AKE_AST_BOOLEAN:
                Zinc_string_destroy(&n->data.boolean.value);
                break;
            case AKE_AST_PARENTHESIS:
                Ake_AstDestroy(n->data.parenthesis.expr);
                break;
            case AKE_AST_TYPE:
                break;
            case AKE_AST_POWER:
                Ake_AstDestroy(n->data.power.left);
                Ake_AstDestroy(n->data.power.right);
                break;
            case AKE_AST_DOT:
                Ake_AstDestroy(n->data.dot.left);
                Ake_AstDestroy(n->data.dot.right);
                break;
            case AKE_AST_STRUCT:
                Ake_AstListDestroy(&n->data._struct_.list);
                break;
            case AKE_AST_RETURN:
                Ake_AstDestroy(n->data._return_.expr);
                break;
            case AKE_AST_PROTOTYPE:
                Ake_AstDestroy(n->data.prototype.id);
                Ake_AstDestroy(n->data.prototype.dseq);
                Ake_AstDestroy(n->data.prototype.ret);
                break;
            default:
                p = n->head;
                while (p) {
                    Ake_Ast* temp = p;
                    p = p->next;
                    Ake_AstDestroy(temp);
                }
                break;
        }

        Zinc_string_destroy(&n->struct_value);
        Ake_TypeDestroy(n->type);
        free(n->type);

        Ake_token_list_destroy(&n->token_list);

        free(n);
    }
}

void Ake_AstAdd(Ake_Ast* p, Ake_Ast* c)
{
    Ake_AstValidate(p);
    Ake_AstValidate(c);

    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
    Zinc_location_combine(&p->loc, &c->loc);
    c->parent = p;
    if (c->has_error) {
        p->has_error = true;
    }
}

void Ake_AstAdd2(Ake_Ast* p, Ake_Ast* c)
{
    Ake_AstValidate(p);
    Ake_AstValidate(c);

    Zinc_location_combine(&p->loc, &c->loc);
    c->parent = p;
    if (c->has_error) {
        p->has_error = true;
    }
}

Ake_Ast* Ake_AstGet(Ake_Ast* p, size_t pos)
{
    Ake_AstValidate(p);

    int i = 0;
    for (Ake_Ast* n = p->head; n; n = n->next) {
        if (i == pos) {
            return n;
        }
        i++;
    }
    return NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_AstCopy(Ake_Ast* src, Ake_Ast* dest)
{
    Ake_AstValidate(src);
    Ake_AstValidate(dest);

    Ake_AstSet(dest, src->kind);
    dest->type = Ake_TypeClone(src->type);
    dest->loc = src->loc;
    Ake_Ast* p = NULL;
    switch (src->kind) {
        case AKE_AST_ID:
            Zinc_string_add_string(&src->data.id.value, &dest->data.id.value);
            break;
        case AKE_AST_SIGN:
            dest->data.sign.right = Ake_AstClone(src->data.sign.right);
            break;
        case AKE_AST_NUMBER:
            Zinc_string_add_string(&src->data.number.value, &dest->data.number.value);
            break;
        case AKE_AST_STRING:
            Zinc_string_add_string(&src->data.string.value, &dest->data.string.value);
            break;
        case AKE_AST_ASSIGN:
            dest->data.assign.left = Ake_AstClone(src->data.assign.left);
            dest->data.assign.right = Ake_AstClone(src->data.assign.right);
            break;
        case AKE_AST_PLUS:
            dest->data.plus.left = Ake_AstClone(src->data.plus.left);
            dest->data.plus.right = Ake_AstClone(src->data.plus.right);
            break;
        case AKE_AST_MINUS:
            dest->data.minus.left = Ake_AstClone(src->data.minus.left);
            dest->data.minus.right = Ake_AstClone(src->data.minus.right);
            break;
        case AKE_AST_MULT:
            dest->data.mult.left = Ake_AstClone(src->data.mult.left);
            dest->data.mult.right = Ake_AstClone(src->data.mult.right);
            break;
        case AKE_AST_DIVIDE:
            dest->data.divide.left = Ake_AstClone(src->data.divide.left);
            dest->data.divide.right = Ake_AstClone(src->data.divide.right);
            break;
        case AKE_AST_STMTS:
            p = src->data.stmts.list.head;
            while (p) {
                Ake_AstListAdd(&dest->data.stmts.list, Ake_AstClone(p));
                p = p->next;
            }
            break;
        case AKE_AST_FUNCTION:
            dest->data.function.proto = Ake_AstClone(src->data.function.proto);
            dest->data.function.body = Ake_AstClone(src->data.function.body);
            break;
        case AKE_AST_DSEQ:
            p = src->data.dseq.list.head;
            while (p) {
                Ake_AstListAdd(&dest->data.dseq.list, Ake_AstClone(p));
                p = p->next;
            }
            break;
        case AKE_AST_DRET:
            dest->data.dret.node = Ake_AstClone(src->data.dret.node);
            break;
        case AKE_AST_CALL:
            dest->data.call.func = Ake_AstClone(src->data.call.func);
            p = src->data.call.args.head;
            while (p) {
                Ake_AstListAdd(&dest->data.call.args, Ake_AstClone(p));
                p = p->next;
            }
            break;
        case AKE_AST_IF:
            p = src->data._if_.branches.head;
            while (p) {
                Ake_AstListAdd(&dest->data._if_.branches, Ake_AstClone(p));
                p = p->next;
            }
            break;
        case AKE_AST_COND_BRANCH:
            dest->data.cond_branch.cond = Ake_AstClone(src->data.cond_branch.cond);
            dest->data.cond_branch.body = Ake_AstClone(src->data.cond_branch.body);
            break;
        case AKE_AST_DEFAULT_BRANCH:
            dest->data.default_branch.body = Ake_AstClone(src->data.default_branch.body);
            break;
        case AKE_AST_EQUALITY:
            dest->data.equality.left = Ake_AstClone(src->data.equality.left);
            dest->data.equality.right = Ake_AstClone(src->data.equality.right);
            break;
        case AKE_AST_NOT_EQUAL:
            dest->data.not_equal.left = Ake_AstClone(src->data.not_equal.left);
            dest->data.not_equal.right = Ake_AstClone(src->data.not_equal.right);
            break;
        case AKE_AST_LESS_THAN:
            dest->data.less_than.left = Ake_AstClone(src->data.less_than.left);
            dest->data.less_than.right = Ake_AstClone(src->data.less_than.right);
            break;
        case AKE_AST_LESS_THAN_OR_EQUAL:
            dest->data.less_than_or_equal.left = Ake_AstClone(src->data.less_than_or_equal.left);
            dest->data.less_than_or_equal.right = Ake_AstClone(src->data.less_than_or_equal.right);
            break;
        case AKE_AST_GREATER_THAN:
            dest->data.greater_than.left = Ake_AstClone(src->data.greater_than.left);
            dest->data.greater_than.right = Ake_AstClone(src->data.greater_than.right);
            break;
        case AKE_AST_GREATER_THAN_OR_EQUAL:
            dest->data.greater_than_or_equal.left = Ake_AstClone(src->data.greater_than_or_equal.left);
            dest->data.greater_than_or_equal.right = Ake_AstClone(src->data.greater_than_or_equal.right);
            break;
        case AKE_AST_NOT:
            dest->data._not_.right = Ake_AstClone(src->data._not_.right);
            break;
        case AKE_AST_AND:
            dest->data._and_.left = Ake_AstClone(src->data._and_.left);
            dest->data._and_.right = Ake_AstClone(src->data._and_.right);
            break;
        case AKE_AST_OR:
            dest->data._or_.left = Ake_AstClone(src->data._or_.left);
            dest->data._or_.right = Ake_AstClone(src->data._or_.right);
            break;
        case AKE_AST_WHILE:
            dest->data._while_.cond = Ake_AstClone(src->data._while_.cond);
            dest->data._while_.body = Ake_AstClone(src->data._while_.body);
            break;
        case AKE_AST_FOR_RANGE:
            dest->data.for_range.dec = Ake_AstClone(src->data.for_range.dec);
            dest->data.for_range.start = Ake_AstClone(src->data.for_range.start);
            dest->data.for_range.end = Ake_AstClone(src->data.for_range.end);
            dest->data.for_range.body = Ake_AstClone(src->data.for_range.body);
            break;
        case AKE_AST_FOR_ITERATION:
            dest->data.for_iteration.dec = Ake_AstClone(src->data.for_iteration.dec);
            dest->data.for_iteration.iterator = Ake_AstClone(src->data.for_iteration.iterator);
            dest->data.for_iteration.body = Ake_AstClone(src->data.for_iteration.body);
        case AKE_AST_DECLARATION:
            dest->data.declaration.id_node = Ake_AstClone(src->data.declaration.id_node);
            dest->data.declaration.type_node = Ake_AstClone(src->data.declaration.type_node);
            break;
        case AKE_AST_ARRAY_LITERAL:
            p = src->data.array_literal.list.head;
            while (p) {
                Ake_AstListAdd(&dest->data.array_literal.list, Ake_AstClone(p));
                p = p->next;
            }
            break;
        case AKE_AST_ARRAY_SUBSCRIPT:
            dest->data.array_subscript.array = Ake_AstClone(src->data.array_subscript.array);
            dest->data.array_subscript.index = Ake_AstClone(src->data.array_subscript.index);
            break;
        case AKE_AST_BOOLEAN:
            Zinc_string_add_string(&dest->data.boolean.value, &src->data.boolean.value);
            break;
        case AKE_AST_PARENTHESIS:
            dest->data.parenthesis.expr = Ake_AstClone(src->data.parenthesis.expr);
            break;
        case AKE_AST_TYPE:
            break;
        case AKE_AST_POWER:
            dest->data.power.left = Ake_AstClone(src->data.power.left);
            dest->data.power.right = Ake_AstClone(src->data.power.right);
            break;
        case AKE_AST_DOT:
            dest->data.dot.left = Ake_AstClone(src->data.dot.left);
            dest->data.dot.right = Ake_AstClone(src->data.dot.right);
            break;
        case AKE_AST_STRUCT:
            p = src->data._struct_.list.head;
            while (p) {
                Ake_AstListAdd(&dest->data._struct_.list, Ake_AstClone(p));
                p = p->next;
            }
            break;
        case AKE_AST_RETURN:
            dest->data._return_.expr = Ake_AstClone(src->data._return_.expr);
            break;
        case AKE_AST_PROTOTYPE:
            dest->data.prototype.id = Ake_AstClone(src->data.prototype.id);
            dest->data.prototype.dseq = Ake_AstClone(src->data.prototype.dseq);
            dest->data.prototype.ret = Ake_AstClone(src->data.prototype.ret);
            break;
        default:
            break;
    }
    Zinc_string_copy(&src->struct_value, &dest->struct_value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_AstClone(Ake_Ast* n)
{
    Ake_AstValidate(n);

    Ake_Ast* copy = NULL;

    if (n) {
        Ake_AstCreate(&copy);
        Ake_AstCopy(n, copy);

        Ake_Ast* p = n->head;
        while (p) {
            Ake_Ast* p_copy = NULL;
            p_copy = Ake_AstClone(p);
            Ake_AstAdd(copy, p_copy);
            p = p->next;
        }
    }

    return copy;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_AstMatch(Ake_Ast* a, Ake_Ast* b)
{
    if (a && b) {
        if (a->kind != b->kind) {
            return false;
        }

        Ake_AstValidate(a);
        Ake_AstValidate(b);

        Ake_Ast* a2 = NULL;
        Ake_Ast* b2 = NULL;

        switch (a->kind) {
            case AKE_AST_ID:
                if (!Zinc_string_compare(&a->data.id.value, &b->data.id.value)) {
                    return false;
                }
                break;
            case AKE_AST_SIGN:
                if (!Ake_AstMatch(a->data.sign.right, b->data.sign.right)) {
                    return false;
                }
                break;
            case AKE_AST_NUMBER:
                if (!Zinc_string_compare(&a->data.number.value, &b->data.number.value)) {
                    return false;
                }
                break;
            case AKE_AST_STRING:
                if (!Zinc_string_compare(&a->data.string.value, &b->data.string.value)) {
                    return false;
                }
                break;
            case AKE_AST_ASSIGN:
                if (!Ake_AstMatch(a->data.assign.left, b->data.assign.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.assign.right, b->data.assign.right)) {
                    return false;
                }
                break;
            case AKE_AST_PLUS:
                if (!Ake_AstMatch(a->data.plus.left, b->data.plus.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.plus.right, b->data.plus.right)) {
                    return false;
                }
                break;
            case AKE_AST_MINUS:
                if (!Ake_AstMatch(a->data.minus.left, b->data.minus.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.minus.right, b->data.minus.right)) {
                    return false;
                }
                break;
            case AKE_AST_MULT:
                if (!Ake_AstMatch(a->data.mult.left, b->data.mult.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.mult.right, b->data.mult.right)) {
                    return false;
                }
                break;
            case AKE_AST_DIVIDE:
                if (!Ake_AstMatch(a->data.divide.left, b->data.divide.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.divide.right, b->data.divide.right)) {
                    return false;
                }
                break;
            case AKE_AST_STMTS:
                a2 = a->data.stmts.list.head;
                b2 = b->data.stmts.list.head;
                while (a2 || b2) {
                    if (!a2) {
                        return false;
                    }
                    if (!b2) {
                        return false;
                    }
                    if (!Ake_AstMatch(a2, b2)) {
                        return false;
                    }
                    a2 = a2->next;
                    b2 = b2->next;
                }
                break;
            case AKE_AST_FUNCTION:
                if (!Ake_AstMatch(a->data.function.proto, b->data.function.proto)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.function.body, b->data.function.body)) {
                    return false;
                }
                break;
            case AKE_AST_DSEQ:
                a2 = a->data.dseq.list.head;
                b2 = b->data.dseq.list.head;
                while (a2 || b2) {
                    if (!a2) {
                        return false;
                    }
                    if (!b2) {
                        return false;
                    }
                    if (!Ake_AstMatch(a2, b2)) {
                        return false;
                    }
                    a2 = a2->next;
                    b2 = b2->next;
                }
                break;
            case AKE_AST_DRET:
                if (!Ake_AstMatch(a->data.dret.node, b->data.dret.node)) {
                    return false;
                }
                break;
            case AKE_AST_CALL:
                if (!Ake_AstMatch(a->data.call.func, b->data.call.func)) {
                    return false;
                }
                a2 = a->data.call.args.head;
                b2 = b->data.call.args.head;
                while (a2 || b2) {
                    if (!a2) {
                        return false;
                    }
                    if (!b2) {
                        return false;
                    }
                    if (!Ake_AstMatch(a2, b2)) {
                        return false;
                    }
                    a2 = a2->next;
                    b2 = b2->next;
                }
                break;
            case AKE_AST_IF:
                a2 = a->data._if_.branches.head;
                b2 = b->data._if_.branches.head;
                while (a2 || b2) {
                    if (!a2) {
                        return false;
                    }
                    if (!b2) {
                        return false;
                    }
                    if (!Ake_AstMatch(a2, b2)) {
                        return false;
                    }
                    a2 = a2->next;
                    b2 = b2->next;
                }
                break;
            case AKE_AST_COND_BRANCH:
                if (!Ake_AstMatch(a->data.cond_branch.cond, b->data.cond_branch.cond)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.cond_branch.body, b->data.cond_branch.body)) {
                    return false;
                }
                break;
            case AKE_AST_DEFAULT_BRANCH:
                if (!Ake_AstMatch(a->data.default_branch.body, b->data.default_branch.body)) {
                    return false;
                }
                break;
            case AKE_AST_EQUALITY:
                if (!Ake_AstMatch(a->data.equality.left, b->data.equality.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.equality.right, b->data.equality.right)) {
                    return false;
                }
                break;
            case AKE_AST_NOT_EQUAL:
                if (!Ake_AstMatch(a->data.not_equal.left, b->data.not_equal.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.not_equal.right, b->data.not_equal.right)) {
                    return false;
                }
                break;
            case AKE_AST_LESS_THAN:
                if (!Ake_AstMatch(a->data.less_than.left, b->data.less_than.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.less_than.right, b->data.less_than.right)) {
                    return false;
                }
                break;
            case AKE_AST_LESS_THAN_OR_EQUAL:
                if (!Ake_AstMatch(a->data.less_than_or_equal.left, b->data.less_than_or_equal.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.less_than_or_equal.right, b->data.less_than_or_equal.right)) {
                    return false;
                }
                break;
            case AKE_AST_GREATER_THAN:
                if (!Ake_AstMatch(a->data.greater_than.left, b->data.greater_than.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.greater_than.right, b->data.greater_than.right)) {
                    return false;
                }
                break;
            case AKE_AST_GREATER_THAN_OR_EQUAL:
                if (!Ake_AstMatch(a->data.greater_than_or_equal.left, b->data.greater_than_or_equal.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.greater_than_or_equal.right, b->data.greater_than_or_equal.right)) {
                    return false;
                }
                break;
            case AKE_AST_NOT:
                if (!Ake_AstMatch(a->data._not_.right, b->data._not_.right)) {
                    return false;
                }
                break;
            case AKE_AST_AND:
                if (!Ake_AstMatch(a->data._and_.left, b->data._and_.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data._and_.right, b->data._and_.right)) {
                    return false;
                }
                break;
            case AKE_AST_OR:
                if (!Ake_AstMatch(a->data._or_.left, b->data._or_.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data._or_.right, b->data._or_.right)) {
                    return false;
                }
                break;
            case AKE_AST_WHILE:
                if (!Ake_AstMatch(a->data._while_.cond, b->data._while_.cond)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data._while_.body, b->data._while_.body)) {
                    return false;
                }
                break;
            case AKE_AST_FOR_RANGE:
                if (!Ake_AstMatch(a->data.for_range.dec, b->data.for_range.dec)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.for_range.start, b->data.for_range.start)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.for_range.end, b->data.for_range.end)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.for_range.body, b->data.for_range.body)) {
                    return false;
                }
                break;
            case AKE_AST_FOR_ITERATION:
                if (!Ake_AstMatch(a->data.for_iteration.dec, b->data.for_iteration.dec)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.for_iteration.iterator, b->data.for_iteration.iterator)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.for_iteration.body, b->data.for_iteration.body)) {
                    return false;
                }
                break;
            case AKE_AST_DECLARATION:
                if (!Ake_AstMatch(a->data.declaration.id_node, b->data.declaration.id_node)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.declaration.type_node, b->data.declaration.type_node)) {
                    return false;
                }
                break;
            case AKE_AST_ARRAY_LITERAL:
                a2 = a->data.array_literal.list.head;
                b2 = b->data.array_literal.list.head;
                while (a2 || b2) {
                    if (!a2) {
                        return false;
                    }
                    if (!b2) {
                        return false;
                    }
                    if (!Ake_AstMatch(a2, b2)) {
                        return false;
                    }
                    a2 = a2->next;
                    b2 = b2->next;
                }
                break;
            case AKE_AST_ARRAY_SUBSCRIPT:
                if (!Ake_AstMatch(a->data.array_subscript.array, b->data.array_subscript.array)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.array_subscript.index, b->data.array_subscript.index)) {
                    return false;
                }
                break;
            case AKE_AST_BOOLEAN:
                if (!Zinc_string_compare(&a->data.boolean.value, &b->data.boolean.value)) {
                    return false;
                }
                break;
            case AKE_AST_PARENTHESIS:
                if (!Ake_AstMatch(a->data.parenthesis.expr, b->data.parenthesis.expr)) {
                    return false;
                }
                break;
            case AKE_AST_TYPE:
                break;
            case AKE_AST_POWER:
                if (!Ake_AstMatch(a->data.power.left, b->data.power.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.power.right, b->data.power.right)) {
                    return false;
                }
                break;
            case AKE_AST_DOT:
                if (!Ake_AstMatch(a->data.dot.left, b->data.dot.left)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.dot.right, b->data.dot.right)) {
                    return false;
                }
                break;
            case AKE_AST_STRUCT:
                a2 = a->data._struct_.list.head;
                b2 = b->data._struct_.list.head;
                while (a2 || b2) {
                    if (!a2) {
                        return false;
                    }
                    if (!b2) {
                        return false;
                    }
                    if (!Ake_AstMatch(a2, b2)) {
                        return false;
                    }
                    a2 = a2->next;
                    b2 = b2->next;
                }
                break;
            case AKE_AST_RETURN:
                if (!Ake_AstMatch(a->data._return_.expr, b->data._return_.expr)) {
                    return false;
                }
                break;
            case AKE_AST_PROTOTYPE:
                if (!Ake_AstMatch(a->data.prototype.id, b->data.prototype.id)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.prototype.dseq, b->data.prototype.dseq)) {
                    return false;
                }
                if (!Ake_AstMatch(a->data.prototype.ret, b->data.prototype.ret)) {
                    return false;
                }
                break;
            default:
                if (!Zinc_string_compare(&a->struct_value, &b->struct_value)) {
                    return false;
                }

                if (!Ake_TypeMatch(a->type, b->type, NULL)) {
                    return false;
                }

                Ake_Ast* c = a->head;
                Ake_Ast* d = b->head;
                do {
                    if (!Ake_AstMatch(c, d)) {
                        return false;
                    }
                    if (c) c = c->next;
                    if (d) d = d->next;
                } while (c || d);
                break;
        }

    } else if (!a && !b) {
        return true;
    } else {
        return false;
    }

    return true;
}

size_t Ake_AstCountChildren(Ake_Ast* n)
{
    size_t count = 0;
    if (n) {
        Ake_AstValidate(n);
        Ake_Ast* p = n->head;
        while (p) {
            count++;
            p = p->next;
        }
    }

    return count;
}

void Ake_AstListInit(Ake_AstList* list, Ake_Ast* parent)
{
    list->head = NULL;
    list->tail = NULL;
    Zinc_location_init(&list->loc);
    list->parent = parent;
    list->has_error = true;
}

void Ake_AstListCreate(Ake_AstList** list, Ake_Ast* parent)
{
    Zinc_malloc_safe((void**)list, sizeof (Ake_AstList));
    Ake_AstListInit(*list, parent);
}

void Ake_AstListDestroy(Ake_AstList* list)
{
    Ake_Ast* p = list->head;
    while (p) {
        Ake_Ast* temp = p;
        p = p->next;
        Ake_AstDestroy(temp);
    }
}

void Ake_AstListAdd(Ake_AstList* list, Ake_Ast* n)
{
    Ake_AstValidate(n);

    if (list->head && list->tail) {
        list->tail->next = n;
        n->prev = list->tail;
        list->tail = n;
    } else {
        list->head = n;
        list->tail = n;
    }

    Zinc_location_combine(&list->loc, &n->loc);
    if (list->parent) {
        Zinc_location_combine(&list->parent->loc, &n->loc);
    }
    if (n->has_error) {
        list->has_error = true;
        if (list->parent) {
            list->parent->has_error = true;
        }
    }
}

Ake_Ast* Ake_AstListGet(Ake_AstList* list, size_t index)
{
    size_t i = 0;
    Ake_Ast* p = list->head;
    while (p) {
        if (i == index) {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

size_t Ake_AstListCount(Ake_AstList* list)
{
    size_t count = 0;
    Ake_Ast* p = list->head;
    while (p) {
        count++;
        p = p->next;
    }
    return count;
}
