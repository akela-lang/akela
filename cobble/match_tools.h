#ifndef COBBLE_MATCH_TOOLS_H
#define COBBLE_MATCH_TOOLS_H

#include "String_slice.h"
#include <stdbool.h>
#include "Ast_node.h"
#include "zinc/list.h"
#include "zinc/hash.h"
#include "zinc/hash_map_size_t.h"
#include <stdlib.h>

typedef enum Match_task_status
{
    Match_task_initial,
    Match_task_started,
    Match_task_finished,
} Match_task_status;

typedef struct Match_task {
    Match_task_status status;
    bool matched;
    Ast_node* n;
    Ast_node* p;
    size_t count;
    String_slice start_slice;
    String_slice end_slice;
    bool opposite;
    struct Match_task* parent;
    struct Match_task* next;
    struct Match_task* prev;
} Match_task;

typedef struct Match_task_stack {
    Match_task* top;
    Match_task* bottom;
} Match_task_stack;

typedef struct Stack_node {
    Match_task_stack* mts;
    struct Stack_list* sl;
    Hash_map_size_t groups;
    size_t priority;
    struct Stack_node* next;
    struct Stack_node* prev;
} Stack_node;

typedef struct Stack_list {
    Stack_node* head;
    Stack_node* tail;
    String_slice slice;
    size_t top_priority;
} Stack_list;

void Match_task_init(Match_task* task, Match_task* parent);
void Match_task_destroy(Match_task* task);
void Match_task_create(Match_task** task, Match_task* parent);

void Match_task_stack_init(Match_task_stack* mts, Stack_node* sn);
void Match_task_stack_create(Match_task_stack** mts, Stack_node* sn);
void Match_task_stack_destroy(Match_task_stack* mts);
void Match_task_stack_push(Match_task_stack *mts, Match_task *new_task);
Match_task* Match_task_stack_pop(Match_task_stack* mts);
Match_task* Match_task_stack_remove(Match_task_stack* mts, Match_task* task);
void Match_task_stack_pop_to(Match_task_stack* mts, Match_task* marker);
void Match_task_stack_add_char(Stack_node* sn, Match_task* task, char c);
Match_task_stack* Match_task_stack_clone(Match_task_stack* mts, struct hash_table* ht, Stack_node* sn);

void Stack_node_init(Stack_node* sn);
void Stack_node_create(Stack_node** sn);
void Stack_node_destroy(Stack_node* sn);
Stack_node* Stack_node_clone(Stack_node* sn);
void Stack_node_dump_groups(Stack_node* sn);

void Stack_list_init(Stack_list* sl);
void Stack_list_create(Stack_list** sl);
Stack_node* Stack_list_add(Stack_list* sl, Stack_node* sn);
size_t Stack_list_next_priority(Stack_list* sl);
void Stack_list_remove(Stack_list* sl, Stack_node* sn);
void Stack_list_destroy(Stack_list* sl);

#endif