#ifndef CENTIPEDE_BUILD_DATA_H
#define CENTIPEDE_BUILD_DATA_H

#include "ast.h"

typedef struct Cent_ast_stack Cent_ast_stack;
struct Cent_ast_stack {
    Cent_ast* n;
    Cent_ast_stack* next;
};

typedef struct Cent_build_data Cent_build_data;
struct Cent_build_data {
    Cent_ast_stack* top;
    Zinc_error_list* errors;
};

void Cent_ast_stack_push(Cent_ast_stack** top, Cent_ast* n);
void Cent_ast_stack_pop(Cent_ast_stack** top);
bool Cent_ast_stack_has_node(Cent_ast_stack* top, Cent_ast* n);

void Cent_build_data_init(Cent_build_data *data, Zinc_error_list* errors);
void Cent_build_data_create(Cent_build_data** data, Zinc_error_list* errors);
void Cent_build_data_destroy(Cent_build_data* data);

#endif