#ifndef COBBLE_AST_NODE_H
#define COBBLE_AST_NODE_H

#include "zinc/buffer.h"
#include "zinc/error.h"
#include "zinc/hash_map_size_t.h"

typedef enum Ast_type {
    Ast_type_none,
    Ast_type_empty,
    Ast_type_literal,
    Ast_type_concat,
    Ast_type_union,
    Ast_type_closure,
    Ast_type_positive_closure,
    Ast_type_group,
    Ast_type_repeat,
    Ast_type_repeat_range,
    Ast_type_option,
    Ast_type_error,
    Ast_type_number,
    Ast_type_wildcard,
    Ast_type_begin,
    Ast_type_end,
    Ast_type_escape,
    Ast_type_character_class,
    Ast_type_character_class_opposite,
    Ast_type_character_range,
    Ast_type_character_type_word,
    Ast_type_character_type_word_opposite,
    Ast_type_character_type_digit,
    Ast_type_character_type_digit_opposite,
    Ast_type_character_type_space,
    Ast_type_character_type_space_opposite,
    Ast_type_character_type_newline_opposite,
} Ast_type;

typedef struct Ast_node {
    Ast_type type;
    char c[4];
    int num;
    unsigned long num_value;
    struct location loc;
    bool is_root;
    bool is_group;
    int group;
    struct Ast_node* next;
    struct Ast_node* prev;
    struct Ast_node* head;
    struct Ast_node* tail;
    struct Ast_node* parent;
} Ast_node;

typedef struct Ast_node_clone_result {
    Ast_node* n;
    Ast_node* ref;
} Ast_node_clone_result;

void Ast_node_init(Ast_node* gn);
void Ast_node_create(Ast_node** n);
void Ast_node_destroy(Ast_node* gn);
void Ast_node_add(Ast_node* p, Ast_node* c);
Ast_node* Ast_node_get(Ast_node* n, int index);
void Ast_node_copy(Ast_node* src, Ast_node* dest);
Ast_node_clone_result Ast_node_clone(Ast_node* n, Ast_node* ref);
void Ast_node_add_char(Ast_node* n, char c);
void Ast_node_set_root(Ast_node* n);
void Ast_node_set_group(Ast_node* n);

#endif //COBBLE_AST_NODE_H
