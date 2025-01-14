#ifndef COBBLE_AST_NODE_H
#define COBBLE_AST_NODE_H

#include "zinc/zstring.h"
#include "zinc/error.h"
#include "zinc/hash_map_size_t.h"

typedef enum Cob_ast_type {
    Cob_ast_type_none,
    Cob_ast_type_empty,
    Cob_ast_type_literal,
    Cob_ast_type_concat,
    Cob_ast_type_union,
    Cob_ast_type_closure,
    Cob_ast_type_positive_closure,
    Cob_ast_type_group,
    Cob_ast_type_repeat,
    Cob_ast_type_repeat_range,
    Cob_ast_type_option,
    Cob_ast_type_error,
    Cob_ast_type_number,
    Cob_ast_type_wildcard,
    Cob_ast_type_begin,
    Cob_ast_type_end,
    Cob_ast_type_escape,
    Cob_ast_type_character_class,
    Cob_ast_type_character_class_opposite,
    Cob_ast_type_character_range,
    Cob_ast_type_character_type_word,
    Cob_ast_type_character_type_word_opposite,
    Cob_ast_type_character_type_digit,
    Cob_ast_type_character_type_digit_opposite,
    Cob_ast_type_character_type_space,
    Cob_ast_type_character_type_space_opposite,
    Cob_ast_type_character_type_newline_opposite,
} Cob_ast_type;

typedef struct Cob_ast {
    Cob_ast_type type;
    char c[4];
    int num;
    unsigned long num_value;
    struct Zinc_location loc;
    bool is_root;
    bool is_group;
    size_t group;
    struct Cob_ast* next;
    struct Cob_ast* prev;
    struct Cob_ast* head;
    struct Cob_ast* tail;
    struct Cob_ast* parent;
} Cob_ast;

typedef struct Ast_node_clone_result {
    Cob_ast* n;
    Cob_ast* ref;
} Ast_node_clone_result;

void Cob_ast_init(Cob_ast* gn);
void Cob_ast_create(Cob_ast** n);
void Cob_ast_destroy(Cob_ast* gn);
void Cob_ast_add(Cob_ast* p, Cob_ast* c);
Cob_ast* Cob_ast_get(Cob_ast* n, int index);
void Ast_node_copy(Cob_ast* src, Cob_ast* dest);
Ast_node_clone_result Ast_node_clone(Cob_ast* n, Cob_ast* ref);
void Ast_node_add_char(Cob_ast* n, char c);

#endif //COBBLE_AST_NODE_H
