#include <stdbool.h>
#include "ast.h"
#include "match.h"
#include <assert.h>
#include <zinc/list.h>
#include <zinc/utf8.h>
#include <ctype.h>
#include "match_tools.h"
#include "json/lex_tools.h"

bool Cob_match_slice(Cob_ast* root, Cob_stack_list* sl, struct buffer_list* groups);
Cob_stack_list* Cob_init_stacks(Cob_re* re, String_slice slice);
void Check_group_tasks(Cob_ast* root, Cob_stack_node* sn, Cob_task* task);
void Cob_remove_finished(Cob_stack* mts, Cob_task* task);
void Cob_cleanup_finished(Cob_task* task);
void Cob_check_if_done(Cob_stack* mts, Cob_task* task, bool* done, bool* matched);
void Cob_get_groups(Cob_stack_node* sn, struct buffer_list* groups);
Cob_task* Cob_add_task(
    Cob_ast* n,
    String_slice slice,
    Cob_stack* mts,
    Cob_task* parent,
    Cob_stack_node* sn);
void Cob_increment_slice(String_slice* slice);
void Cob_run_dispatch(Cob_stack_node* sn);
void Cob_run_union(Cob_stack_node* sn);
void Cob_child_finish_union(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_concat(Cob_stack_node* sn);
void Cob_child_finish_concat(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_closure(Cob_stack_node* sn);
void Cob_child_finish_closure(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_pos_closure(Cob_stack_node* sn);
void Cob_child_finish_pos_closure(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_repeat(Cob_stack_node* sn);
void Cob_child_finish_repeat(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_repeat_range(Cob_stack_node* sn);
void Cob_child_finish_repeat_range(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_group(Cob_stack_node* sn);
void Cob_child_finish_group(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_option(Cob_stack_node* sn);
void Cob_child_finish_option(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_literal(Cob_stack_node* sn);
void Cob_run_wildcard(Cob_stack_node* sn);
void Cob_run_begin(Cob_stack_node* sn);
void Cob_run_end(Cob_stack_node* sn);
void Cob_run_escape(Cob_stack_node* sn);
void Cob_run_character_class(Cob_stack_node* sn);
void Cob_child_finish_character_class(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_character_class_opposite(Cob_stack_node* sn);
void Cob_child_finish_character_class_opposite(Cob_stack_node* sn, Cob_task* parent, Cob_task* child);
void Cob_run_character_range(Cob_stack_node* sn);
void Cob_run_character_type_word(Cob_stack_node* sn, bool opposite);
void Cob_run_character_type_digit(Cob_stack_node* sn, bool opposite);
void Cob_run_character_type_space(Cob_stack_node* sn, bool opposite);
void Cob_run_character_type_newline_opposite(Cob_stack_node* sn);

Cob_result Cob_match(Cob_re* re, String_slice slice)
{
    Cob_result mr;
    Cob_result_init(&mr);

    while (true) {
        Cob_stack_list* sl = Cob_init_stacks(re, slice);
        mr.matched = Cob_match_slice(re->root, sl, &mr.groups);
        Cob_stack_list_destroy(sl);
        if (mr.matched) break;
        if (slice.size == 0) break;
        Cob_increment_slice(&slice);
    }

    return mr;
}

bool Cob_match_slice(Cob_ast* root, Cob_stack_list* sl, struct buffer_list* groups)
{
    bool matched = false;

    while (sl->tail) {
        Cob_stack_node* sn = sl->tail;
        Cob_stack* mts = sn->stack;
        Cob_task* task = mts->top;

        if (task->status == Cob_task_status_finished) {
            bool stack_done = false;
            Cob_remove_finished(mts, task);
            Cob_check_if_done(mts, task, &stack_done, &matched);
            Cob_cleanup_finished(task);

            if (stack_done) {
                if (matched) {
                    Cob_get_groups(sn, groups);
                    sn = sl->head;
                    while (sn) {
                        Cob_stack_node* temp = sn;
                        sn = sn->next;
                        Cob_stack_list_remove(sl, temp);
                        Cob_stack_node_destroy(temp);
                    }
                } else {
                    Cob_stack_list_remove(sl, sn);
                    Cob_stack_node_destroy(sn);
                }
            }
        } else {
            Cob_run_dispatch(sn);
        }
    }

    return matched;
}

Cob_stack_list* Cob_init_stacks(Cob_re* re, String_slice slice)
{
    Cob_stack_list* sl;
    Cob_stack_list_create(&sl);
    sl->slice = slice;

    Cob_stack* mts = NULL;
    Cob_stack_create(&mts, NULL);

    Cob_stack_node* sn = NULL;
    Cob_stack_node_create(&sn);
    sn->stack = mts;
    sn->sl = sl;
    for (size_t i = 0; i < re->group_count; i++) {
        struct buffer* bf = NULL;
        buffer_create(&bf);
        Hash_map_size_t_add(&sn->groups, i, bf);
    }

    Cob_stack_list_add(sl, sn);

    Cob_add_task(re->root, slice, mts, NULL, sn);

    return sl;
}

void Cob_remove_finished(Cob_stack* mts, Cob_task* task)
{
    if (task->status == Cob_task_status_finished) {
        assert(task == mts->top);
        Cob_stack_pop(mts);
    }
}

void Cob_cleanup_finished(Cob_task* task)
{
    if (task->status == Cob_task_status_finished) {
        free(task);
    }
}

void Cob_check_if_done(Cob_stack* mts, Cob_task* task, bool* done, bool* matched)
{
    if (mts->top == NULL) {
        *done = true;
        if (task->matched) {
            // if (task->end_slice.size == 0) {
                *matched = true;
            // }
        }
    }
}

void Cob_get_groups(Cob_stack_node* sn, struct buffer_list* groups)
{
    size_t index = 0;
    while (true) {
        struct buffer* bf = Hash_map_size_t_get(&sn->groups, index);
        if (!bf) break;
        buffer_list_add_bf(groups, bf);
        index++;
    }
}

Cob_task* Cob_add_task(
    Cob_ast* n,
    String_slice slice,
    Cob_stack* mts,
    Cob_task* parent,
    Cob_stack_node* sn)
{
    Cob_task* task = NULL;
    Cob_task_create(&task, parent);
    task->n = n;
    task->start_slice = slice;
    Cob_stack_push(mts, task);
    return task;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_run_dispatch(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    assert(task);
    assert(task->status != Cob_task_status_finished);

    if (!task->n) {
        if (task->start_slice.size == 0) {
            task->matched = true;
        }
        task->status = Cob_task_status_finished;
        struct buffer* bf = NULL;
        buffer_create(&bf);
        Hash_map_size_t_add(&sn->groups, 0, bf);
    } else if (task->n->type == Cob_ast_type_union) {
        Cob_run_union(sn);
    } else if (task->n->type == Cob_ast_type_concat) {
        Cob_run_concat(sn);
    } else if (task->n->type == Cob_ast_type_closure) {
        Cob_run_closure(sn);
    } else if (task->n->type == Cob_ast_type_positive_closure) {
        Cob_run_pos_closure(sn);
    } else if (task->n->type == Cob_ast_type_repeat) {
        Cob_run_repeat(sn);
    } else if (task->n->type == Cob_ast_type_repeat_range) {
        Cob_run_repeat_range(sn);
    } else if (task->n->type == Cob_ast_type_group) {
        Cob_run_group(sn);
    } else if (task->n->type == Cob_ast_type_option) {
        Cob_run_option(sn);
    } else if (task->n->type == Cob_ast_type_literal) {
        Cob_run_literal(sn);
    } else if (task->n->type == Cob_ast_type_wildcard) {
        Cob_run_wildcard(sn);
    } else if (task->n->type == Cob_ast_type_begin) {
        Cob_run_begin(sn);
    } else if (task->n->type == Cob_ast_type_end) {
        Cob_run_end(sn);
    } else if (task->n->type == Cob_ast_type_escape) {
        Cob_run_escape(sn);
    } else if (task->n->type == Cob_ast_type_character_class) {
        Cob_run_character_class(sn);
    } else if (task->n->type == Cob_ast_type_character_class_opposite) {
        Cob_run_character_class_opposite(sn);
    } else if (task->n->type == Cob_ast_type_character_range) {
        Cob_run_character_range(sn);
    } else if (task->n->type == Cob_ast_type_character_type_word) {
        Cob_run_character_type_word(sn, false);
    } else if (task->n->type == Cob_ast_type_character_type_word_opposite) {
        Cob_run_character_type_word(sn, true);
    } else if (task->n->type == Cob_ast_type_character_type_digit) {
        Cob_run_character_type_digit(sn, false);
    } else if (task->n->type == Cob_ast_type_character_type_digit_opposite) {
        Cob_run_character_type_digit(sn, true);
    } else if (task->n->type == Cob_ast_type_character_type_space) {
        Cob_run_character_type_space(sn, false);
    } else if (task->n->type == Cob_ast_type_character_type_space_opposite) {
        Cob_run_character_type_space(sn, true);
    } else if (task->n->type == Cob_ast_type_character_type_newline_opposite) {
        Cob_run_character_type_newline_opposite(sn);
    } else {
        assert(false && "Not implemented");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_dispatch(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (parent) {
        if (parent->n) {
            if (parent->n->type == Cob_ast_type_union) {
                Cob_child_finish_union(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_concat) {
                Cob_child_finish_concat(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_closure) {
                Cob_child_finish_closure(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_positive_closure) {
                Cob_child_finish_pos_closure(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_repeat) {
                Cob_child_finish_repeat(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_repeat_range) {
                Cob_child_finish_repeat_range(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_group) {
                Cob_child_finish_group(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_option) {
                Cob_child_finish_option(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_literal) {
                assert(false && "literal tasks should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_wildcard) {
                assert(false && "wildcard tasks should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_begin) {
                assert(false && "begin tasks should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_end) {
                assert(false && "end tasks should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_escape) {
                assert(false && "escape tasks should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_class) {
                Cob_child_finish_character_class(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_character_class_opposite) {
                Cob_child_finish_character_class_opposite(sn, parent, child);
            } else if (parent->n->type == Cob_ast_type_character_range) {
                assert(false && "character range task should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_type_word) {
                assert(false && "character type word task should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_type_word_opposite) {
                assert(false && "character type word opposite task should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_type_digit) {
                assert(false && "character type digit task should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_type_digit_opposite) {
                assert(false && "character type digit opposite task should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_type_space) {
                assert(false && "character type space task should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_type_space_opposite) {
                assert(false && "character type space opposite task should not have child tasks");
            } else if (parent->n->type == Cob_ast_type_character_type_newline_opposite) {
                assert(false && "character type newline opposite task should not have child tasks");
            } else {
                assert(false && "invalid type");
            }
        }
    }
}

void Cob_run_union(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;

    if (task->status == Cob_task_status_initial) {
        task->p = task->n->head;
        assert(task->p);
        task->status = Cob_task_status_started;
        task->end_slice = task->start_slice;
        task->matched = false;

        Cob_ast* p = task->p->next;
        while (p) {
            Cob_stack_node* new_sn = Cob_stack_node_clone(sn);
            new_sn->stack->top->p = p;
            Cob_stack_list_add(sn->sl, new_sn);
            Cob_add_task(
                new_sn->stack->top->p,
                new_sn->stack->top->start_slice,
                new_sn->stack,
                new_sn->stack->top,
                new_sn);
            p = p->next;
        }

        Cob_add_task(task->p, task->start_slice, sn->stack, task, sn);
    } else {
        assert(false && "should not happen");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_union(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    parent->matched = child->matched;
    parent->status = Cob_task_status_finished;
    if (child->matched) {
        parent->end_slice = child->end_slice;
    }
    Cob_child_finish_dispatch(sn, parent->parent, parent);
}

void Cob_run_concat(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    if (task->status == Cob_task_status_initial) {
        task->p = task->n->head;
        task->status = Cob_task_status_started;
        task->end_slice = task->start_slice;
        task->matched = true;
    }

    if (task->p) {
        task->status = Cob_task_status_started;
        Cob_add_task(task->p, task->end_slice, sn->stack, task, sn);
        task->p = task->p->next;
    } else {
        task->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_concat(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    parent->matched = parent->matched && child->matched;
    if (child->matched) {
        parent->end_slice = child->end_slice;
    } else {
        parent->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void Cob_run_closure(Cob_stack_node* sn)
{
    Cob_task *task = sn->stack->top;

    if (task->status == Cob_task_status_initial) {
        task->end_slice = task->start_slice;
        task->status = Cob_task_status_started;
        task->matched = true;
    }

    if (task->status != Cob_task_status_finished) {
        Cob_stack_node* new_sn = Cob_stack_node_clone(sn);
        Cob_task* new_task = new_sn->stack->top;
        Cob_add_task(new_task->n->head, new_task->end_slice, new_sn->stack, new_task, new_sn);
        new_sn->priority = Cob_stack_list_next_priority(new_sn->sl);
        Cob_stack_list_add(sn->sl, new_sn);

        task->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_closure(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
    } else {
        parent->matched = false;
        parent->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void Cob_run_pos_closure(Cob_stack_node* sn)
{
    Cob_task *task = sn->stack->top;

    if (task->status == Cob_task_status_initial) {
        task->end_slice = task->start_slice;
        task->status = Cob_task_status_started;

        Cob_add_task(task->n->head, task->end_slice, sn->stack, task, sn);
    } else {
        Cob_stack_node* new_sn = Cob_stack_node_clone(sn);
        Cob_task* new_task = new_sn->stack->top;
        Cob_add_task(new_task->n->head, new_task->end_slice, new_sn->stack, new_task, new_sn);
        new_sn->priority = Cob_stack_list_next_priority(new_sn->sl);
        Cob_stack_list_add(sn->sl, new_sn);

        task->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_pos_closure(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->matched = true;
    } else {
        parent->status = Cob_task_status_finished;
        // parent->matched = true;
        Cob_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void Cob_run_repeat(Cob_stack_node* sn)
{
    Cob_task *task = sn->stack->top;
    if (task->status == Cob_task_status_initial) {
        task->end_slice = task->start_slice;
        task->status = Cob_task_status_started;
    }

    Cob_ast* child_node = task->n->head;
    Cob_ast* repeat_node = child_node->next;
    if (repeat_node->num_value == 0) {
        task->status = Cob_task_status_finished;
        task->matched = true;
    }

    if (task->count < repeat_node->num_value) {
        Cob_add_task(child_node, task->end_slice, sn->stack, task, sn);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_repeat(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    Cob_ast* child_node = parent->n->head;
    Cob_ast* repeat_node = child_node->next;

    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->count++;
        if (repeat_node->num_value == parent->count) {
            parent->matched = true;
            parent->status = Cob_task_status_finished;
        }
    } else {
        parent->status = Cob_task_status_finished;
    }

    Cob_child_finish_dispatch(sn, parent->parent, parent);
}

void Cob_run_repeat_range(Cob_stack_node* sn)
{
    Cob_task *task = sn->stack->top;
    if (task->status == Cob_task_status_initial) {
        task->end_slice = task->start_slice;
        task->status = Cob_task_status_started;
    }

    Cob_ast* child_node = task->n->head;
    Cob_ast* repeat_low = child_node->next;
    Cob_ast* repeat_high = repeat_low->next;

    if (task->count >= repeat_low->num_value && task->count <= repeat_high->num_value) {
        Cob_stack_node* new_sn = Cob_stack_node_clone(sn);
        Cob_task* new_task = new_sn->stack->top;
        Cob_add_task(new_task->n->head, new_task->end_slice, new_sn->stack, new_task, new_sn);
        Cob_stack_list_add(sn->sl, new_sn);

        task->status = Cob_task_status_finished;
        task->matched = true;
        Cob_child_finish_dispatch(sn, task->parent, task);
    } else if (task->count < repeat_high->num_value) {
        Cob_add_task(child_node, task->end_slice, sn->stack, task, sn);
    } else {
        task->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_repeat_range(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->count++;
    } else {
        parent->status = Cob_task_status_finished;
    }

    Cob_child_finish_dispatch(sn, parent->parent, parent);
}

void Cob_run_group(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    if (task->n->head) {
        Cob_add_task(task->n->head, task->start_slice, sn->stack, task, sn);
        task->status = Cob_task_status_started;
    } else {
        task->status = Cob_task_status_finished;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_group(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (child->matched) {
        parent->matched = true;
        parent->end_slice = child->end_slice;
    }
    parent->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, parent->parent, parent);
}

void Cob_run_option(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    if (task->status == Cob_task_status_initial) {
        task->end_slice = task->start_slice;
        task->status = Cob_task_status_started;
    }

    if (task->count == 0) {
        Cob_stack_node* new_sn = Cob_stack_node_clone(sn);
        Cob_task* new_task = new_sn->stack->top;
        Cob_add_task(new_task->n->head, new_task->end_slice, new_sn->stack, new_task, new_sn);
        new_sn->priority = Cob_stack_list_next_priority(new_sn->sl);
        Cob_stack_list_add(sn->sl, new_sn);

        task->status = Cob_task_status_finished;
        task->matched = true;
        Cob_child_finish_dispatch(sn, task->parent, task);
    } else if (task->count == 1) {
        task->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, task->parent, task);
    } else {
        assert(false && "option count should be 0 or 1");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_option(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->count++;
        parent->matched = true;
    } else {
        parent->status = Cob_task_status_finished;
        Cob_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void Cob_increment_slice(String_slice* slice)
{
    int num = NUM_BYTES(slice->p[0]);
    slice->p += num;
    slice->size -= num;
}

void Cob_run_literal(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        bool matched = true;
        int num = NUM_BYTES(slice.p[0]);
        if (task->n->num == num) {
            for (int i = 0; i < num; i++) {
                if (task->n->c[i] != slice.p[i]) {
                    matched = false;
                    break;
                }
            }
        } else {
            matched = false;
        }
        if (task->opposite) {
            matched = !matched;
        }
        if (matched) {
            if (!task->opposite) {
                Cob_stack_node_add_char(sn, task, slice);
                Cob_increment_slice(&slice);
            }
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_wildcard(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;

    String_slice slice = task->start_slice;
    int num = NUM_BYTES(slice.p[0]);
    if (slice.size > 0) {
        if (!(num == 1 && slice.p[0] == '\n')) {
            Cob_stack_node_add_char(sn, task, slice);
            Cob_increment_slice(&slice);
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_begin(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    task->end_slice = task->start_slice;
    if (task->end_slice.p == sn->sl->slice.p) {
        task->matched = true;
    }
    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_end(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    task->end_slice = task->start_slice;

    if (task->end_slice.size == 0) {
        task->matched = true;
    }
    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_escape(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        bool matched = true;
        int num = NUM_BYTES(slice.p[0]);
        if (task->n->num == num) {
            for (int i = 0; i < num; i++) {
                if (task->n->c[i] != slice.p[i]) {
                    matched = false;
                    break;
                }
            }
        } else {
            matched = false;
        }
        if (task->opposite) {
            matched = !matched;
        }
        if (matched) {
            for (int i = 0; i < task->n->num; i++) {
                if (!task->opposite) {
                    Cob_stack_node_add_char(sn, task, slice);
                    Cob_increment_slice(&slice);
                }
            }
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_run_character_class(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    if (task->status == Cob_task_status_initial) {
        task->p = task->n->head;
        task->status = Cob_task_status_started;
        task->end_slice = task->start_slice;
    }

    Cob_ast* p = task->n->head;
    while (p && !task->matched) {
        Cob_task* child = Cob_add_task(p, task->start_slice, sn->stack, task, sn);
        Cob_run_dispatch(sn);
        Cob_remove_finished(sn->stack, child);
        Cob_cleanup_finished(child);
        p = p->next;
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_character_class(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (child->matched) {
        parent->matched = true;
        parent->end_slice = child->end_slice;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_run_character_class_opposite(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;
    if (task->status == Cob_task_status_initial) {
        task->p = task->n->head;
        task->status = Cob_task_status_started;
        task->end_slice = task->start_slice;
        task->matched = true;
    }

    Cob_ast* p = task->n->head;
    while (p && task->matched) {
        Cob_task* child = Cob_add_task(p, task->start_slice, sn->stack, task, sn);
        child->opposite = true;
        Cob_run_dispatch(sn);
        Cob_remove_finished(sn->stack, child);
        Cob_cleanup_finished(child);
        p = p->next;
    }

    if (task->matched) {
        Cob_stack_node_add_char(sn, task, task->end_slice);
        Cob_increment_slice(&task->end_slice);
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_child_finish_character_class_opposite(Cob_stack_node* sn, Cob_task* parent, Cob_task* child)
{
    if (!child->matched) {
        parent->matched = false;
        parent->end_slice = child->end_slice;
    }
}

void Cob_run_character_range(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        Cob_ast* a = task->n->head;
        Cob_ast* b = a->next;
        bool matched = IS_ONE_BYTE(a->c[0]) && IS_ONE_BYTE(b->c[0]) && IS_ONE_BYTE(slice.p[0])
            && slice.p[0] >= a->c[0] && slice.p[0] <= b->c[0];
        if (task->opposite) {
            matched = !matched;
        }
        if (matched) {
            if (!task->opposite) {
                Cob_stack_node_add_char(sn, task, slice);
                Cob_increment_slice(&slice);
            }
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_character_type_word(Cob_stack_node* sn, bool opposite)
{
    Cob_task* task = sn->stack->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        UChar32 cp;
        Json_convert_slice(slice, &cp);
        bool is_word = u_isalpha(cp) || u_isdigit(cp) || cp == '_';
        if ((!opposite && is_word) || (opposite && !is_word)) {
            task->matched = true;
            Cob_stack_node_add_char(sn, task, slice);
            Cob_increment_slice(&slice);
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_character_type_digit(Cob_stack_node* sn, bool opposite)
{
    Cob_task* task = sn->stack->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        UChar32 cp;
        Json_convert_slice(slice, &cp);
        bool is_digit = u_isdigit(cp);
        if ((is_digit && !opposite) || (!is_digit && opposite)) {
            task->matched = true;
            Cob_stack_node_add_char(sn, task, slice);
            Cob_increment_slice(&slice);
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_character_type_space(Cob_stack_node* sn, bool opposite)
{
    Cob_task* task = sn->stack->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        UChar32 cp;
        Json_convert_slice(slice, &cp);
        bool is_space = u_isspace(cp);
        if ((is_space && !opposite) || (!is_space && opposite)) {
            task->matched = true;
            Cob_stack_node_add_char(sn, task, slice);
            Cob_increment_slice(&slice);
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}

void Cob_run_character_type_newline_opposite(Cob_stack_node* sn)
{
    Cob_task* task = sn->stack->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        bool is_space = IS_ONE_BYTE(slice.p[0]) && isspace(slice.p[0]);
        if (!is_space) {
            task->matched = true;
            Cob_stack_node_add_char(sn, task, slice);
            Cob_increment_slice(&slice);
            task->end_slice = slice;
        }
    }

    task->status = Cob_task_status_finished;
    Cob_child_finish_dispatch(sn, task->parent, task);
}