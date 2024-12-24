#ifndef CENTIPEDE_AST_H
#define CENTIPEDE_AST_H

#include "element.h"
#include "enumerate.h"
#include "value.h"
#include "zinc/error.h"
#include "environment.h"

typedef enum Cent_ast_type {
    Cent_ast_type_none,
    Cent_ast_type_element,
    Cent_ast_type_enumerate,
    Cent_ast_type_assign,
    Cent_ast_type_id,
    Cent_ast_type_value,
} Cent_ast_type;

typedef struct Cent_ast {
    Cent_ast_type type;
    union {
        Cent_element* element;
        Cent_enumerate* enumerate;
        Cent_value* value;
    } data;
    Cent_environment* env;
    struct location loc;
    struct Cent_ast* next;
    struct Cent_ast* prev;
    struct Cent_ast* head;
    struct Cent_ast* tail;
    struct Cent_ast* parent;
} Cent_ast;

void Cent_ast_init(Cent_ast *ast);
void Cent_ast_create(Cent_ast **ast);
void Cent_ast_set_type(Cent_ast *ast, Cent_ast_type type);
void Cent_ast_destroy(Cent_ast *ast);
void Cent_ast_add(Cent_ast *p, Cent_ast *c);

#endif