#ifndef WORM_AST_H
#define WORM_AST_H

#include <stdbool.h>
#include "zinc/zstring.h"
#include <stdint.h>
#include "zinc/error.h"

typedef enum Worm_ast_type {
    Worm_ast_type_none,
    Worm_ast_type_stmts,
    Worm_ast_type_id,
    Worm_ast_type_string,
    Worm_ast_type_integer,
    Worm_ast_type_natural,
    Worm_ast_type_real,
    Worm_ast_type_boolean,
    Worm_ast_type_object,
    Worm_ast_type_count,
} Worm_ast_type;

typedef struct Worm_ast {
    Worm_ast_type type;
    Zinc_string value;
    union {
        int64_t integer;
        uint64_t natural;
        double real;
        bool boolean;
    } number;
    Zinc_location loc;
    bool has_error;
    struct Worm_ast* next;
    struct Worm_ast* prev;
    struct Worm_ast* head;
    struct Worm_ast* tail;
} Worm_ast;

void Worm_ast_init(Worm_ast* n);
void Worm_ast_create(Worm_ast** n);
void Worm_ast_add(Worm_ast* p, Worm_ast* c);
void Worm_ast_destroy(Worm_ast* n);

#endif
