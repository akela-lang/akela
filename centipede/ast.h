#ifndef CENTIPEDE_AST_H
#define CENTIPEDE_AST_H

#include "value.h"
#include "zinc/error.h"
#include "environment.h"

typedef enum Cent_ast_type {
    Cent_ast_type_none,
    Cent_ast_type_stmts,
    Cent_ast_type_element_type,
    Cent_ast_type_prop,
    Cent_ast_type_prop_dec,
    Cent_ast_type_children,
    Cent_ast_type_enum_type,
    Cent_ast_type_assign,
    Cent_ast_type_id,
    Cent_ast_type_value,
    Cent_ast_type_modifier,
    Cent_ast_type_object_stmts,
    Cent_ast_type_prop_set,
    Cent_ast_type_value_enum,
    Cent_ast_type_method_child_of,
    Cent_ast_type_method_property_of,
    Cent_ast_type_function_top,
    Cent_ast_type_function_file_name,
} Cent_ast_type;

typedef struct Cent_ast {
    Cent_ast_type type;
    struct buffer text;
    Cent_value_type value_type;
    Cent_number_type number_type;
    Cent_data data;
    struct Cent_environment* env;
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
void Cent_ast_value_set_type(Cent_ast *ast, Cent_value_type type);
void Cent_ast_destroy(Cent_ast *ast);
void Cent_ast_add(Cent_ast *p, Cent_ast *c);
Cent_ast* Cent_ast_get(Cent_ast *n, size_t index);

#endif