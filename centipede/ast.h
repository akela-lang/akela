#ifndef CENTIPEDE_AST_H
#define CENTIPEDE_AST_H

#include "element.h"
#include "enumerate.h"
#include "value.h"
#include "zinc/error.h"
#include "environment.h"

typedef enum Cent_ast_type {
    Cent_ast_type_none,
    Cent_ast_type_stmts,
    Cent_ast_type_element,
    Cent_ast_type_prop,
    Cent_ast_type_prop_dec,
    Cent_ast_type_children,
    Cent_ast_type_enumerate,
    Cent_ast_type_assign,
    Cent_ast_type_id,
    Cent_ast_type_value,
    Cent_ast_type_modifier,
} Cent_ast_type;

typedef struct Cent_ast {
    Cent_ast_type type;
    struct buffer value;
    Cent_environment* env;
    struct location loc;
    bool has_error;
    struct Cent_ast* next;
    struct Cent_ast* prev;
    struct Cent_ast* head;
    struct Cent_ast* tail;
    struct Cent_ast* parent;
} Cent_ast;

void Cent_ast_init(Cent_ast *ast);
void Cent_ast_create(Cent_ast **ast);
void Cent_ast_destroy(Cent_ast *ast);
void Cent_ast_add(Cent_ast *p, Cent_ast *c);
Cent_ast* Cent_ast_get(Cent_ast *n, size_t index);

#endif