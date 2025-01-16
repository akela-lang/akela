#ifndef COBBLE_MATCH_TOOLS_H
#define COBBLE_MATCH_TOOLS_H

#include <stdbool.h>
#include "ast.h"
#include "zinc/hash_map_string.h"
#include "zinc/hash_map_size_t.h"
#include "zinc/String_slice.h"
#include <unicode/uchar.h>
#include "zinc/string_list.h"

typedef enum Cob_task_status {
    Cob_task_status_initial,
    Cob_task_status_started,
    Cob_task_status_finished,
} Cob_task_status;

typedef struct Cob_task {
    Cob_task_status status;
    bool matched;
    Cob_ast* n;
    Cob_ast* p;
    size_t count;
    String_slice start_slice;
    String_slice end_slice;
    bool opposite;
    struct Cob_task* parent;
    struct Cob_task* next;
    struct Cob_task* prev;
} Cob_task;

typedef struct Cob_stack {
    Cob_task* top;
    Cob_task* bottom;
} Cob_stack;

typedef struct Cob_stack_node {
    Cob_stack* stack;
    struct Cob_stack_list* sl;
    Zinc_hash_map_size_t groups;
    size_t priority;
    struct Cob_stack_node* next;
    struct Cob_stack_node* prev;
} Cob_stack_node;

typedef struct Cob_stack_list {
    Cob_stack_node* head;
    Cob_stack_node* tail;
    String_slice slice;
    size_t top_priority;
} Cob_stack_list;

typedef struct Cob_result {
    bool matched;
    struct Zinc_string_list groups;
} Cob_result;

void Cob_task_init(Cob_task* task, Cob_task* parent);
void Cob_task_create(Cob_task** task, Cob_task* parent);

void Cob_stack_init(Cob_stack* mts, Cob_stack_node* sn);
void Cob_stack_create(Cob_stack** mts, Cob_stack_node* sn);
void Cob_stack_destroy(Cob_stack* mts);
void Cob_stack_push(Cob_stack *mts, Cob_task *new_task);
Cob_task* Cob_stack_pop(Cob_stack* mts);
Cob_task* Cob_stack_remove(Cob_stack* mts, Cob_task* task);
void Cob_stack_pop_to(Cob_stack* mts, Cob_task* marker);
void Cob_stack_node_add_char(Cob_stack_node* sn, Cob_task* task, String_slice slice);
Cob_stack* Cob_stack_clone(Cob_stack* mts, struct Zinc_hash_table* ht, Cob_stack_node* sn);

void Cob_stack_node_init(Cob_stack_node* sn);
void Cob_stack_node_create(Cob_stack_node** sn);
void Cob_stack_node_destroy(Cob_stack_node* sn);
Cob_stack_node* Cob_stack_node_clone(Cob_stack_node* sn);
void Cob_stack_node_dump_groups(Cob_stack_node* sn);

void Cob_result_init(Cob_result* mr);
void Cob_result_destroy(Cob_result* mr);

void Cob_stack_list_init(Cob_stack_list* sl);
void Cob_stack_list_create(Cob_stack_list** sl);
Cob_stack_node* Cob_stack_list_add(Cob_stack_list* sl, Cob_stack_node* sn);
size_t Cob_stack_list_next_priority(Cob_stack_list* sl);
void Cob_stack_list_remove(Cob_stack_list* sl, Cob_stack_node* sn);
void Cob_stack_list_destroy(Cob_stack_list* sl);

enum Zinc_result match_convert_char(String_slice slice, UChar32* c);

#endif