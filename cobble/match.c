#include <stdbool.h>
#include "Ast_node.h"
#include "match.h"
#include <assert.h>
#include <zinc/list.h>
#include <zinc/utf8.h>
#include <ctype.h>

#include "match_tools.h"

bool re_match_slice(Ast_node* root, Stack_list* sl, struct buffer_list* groups);
Stack_list* Init_stacks(Ast_node* root, String_slice slice);
void Check_group_tasks(Ast_node* root, Stack_node* sn, Match_task* task);
void Remove_finished_task(Match_task_stack* mts, Match_task* task);
void Cleanup_finished_task(Match_task* task);
void Check_if_done(Match_task_stack* mts, Match_task* task, bool* done, bool* matched);
void Get_groups(Stack_node* sn, struct buffer_list* groups);
Match_task* re_match_add_task(
    Ast_node* n,
    String_slice slice,
    Match_task_stack* mts,
    Match_task* parent,
    Stack_node* sn);
void Increment_slice(String_slice* slice);
void re_match_run_dispatch(Stack_node* sn);
void re_match_run_union(Stack_node* sn);
void re_match_child_finish_union(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_concat(Stack_node* sn);
void re_match_child_finish_concat(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_closure(Stack_node* sn);
void re_match_child_finish_closure(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_pos_closure(Stack_node* sn);
void re_match_child_finish_pos_closure(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_repeat(Stack_node* sn);
void re_match_child_finish_repeat(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_repeat_range(Stack_node* sn);
void re_match_child_finish_repeat_range(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_group(Stack_node* sn);
void re_match_child_finish_group(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_option(Stack_node* sn);
void re_match_child_finish_option(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_literal(Stack_node* sn);
void re_match_run_wildcard(Stack_node* sn);
void re_match_run_begin(Stack_node* sn);
void re_match_run_end(Stack_node* sn);
void re_match_run_escape(Stack_node* sn);
void re_match_run_character_class(Stack_node* sn);
void re_match_child_finish_character_class(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_character_class_opposite(Stack_node* sn);
void re_match_child_finish_character_class_opposite(Stack_node* sn, Match_task* parent, Match_task* child);
void re_match_run_character_range(Stack_node* sn);
void re_match_run_character_type_word(Stack_node* sn, bool opposite);
void re_match_run_character_type_digit(Stack_node* sn, bool opposite);
void re_match_run_character_type_space(Stack_node* sn, bool opposite);

bool re_match(Ast_node* root, String_slice slice, struct buffer_list* groups)
{
    bool matched = false;

    while (true) {
        Stack_list* sl = Init_stacks(root, slice);
        matched = re_match_slice(root, sl, groups);
        Stack_list_destroy(sl);
        if (matched) break;
        if (slice.size == 0) break;
        Increment_slice(&slice);
    }

    return matched;
}

bool re_match_slice(Ast_node* root, Stack_list* sl, struct buffer_list* groups)
{
    bool matched = false;

    while (sl->tail) {
        Stack_node* sn = sl->tail;
        Match_task_stack* mts = sn->mts;
        Match_task* task = mts->top;

        if (task->status == Match_task_finished) {
            bool stack_done = false;
            Remove_finished_task(mts, task);
            Check_if_done(mts, task, &stack_done, &matched);
            Cleanup_finished_task(task);

            if (stack_done) {
                if (matched) {
                    Get_groups(sn, groups);
                    sn = sl->head;
                    while (sn) {
                        Stack_node* temp = sn;
                        sn = sn->next;
                        Stack_list_remove(sl, temp);
                        Stack_node_destroy(temp);
                    }
                } else {
                    Stack_list_remove(sl, sn);
                    Stack_node_destroy(sn);
                }
            }
        } else {
            re_match_run_dispatch(sn);
        }
    }

    return matched;
}

Stack_list* Init_stacks(Ast_node* root, String_slice slice)
{
    Stack_list* sl;
    Stack_list_create(&sl);
    sl->slice = slice;

    Match_task_stack* mts = NULL;
    Match_task_stack_create(&mts, NULL);

    Match_task_stack* finished = NULL;
    Match_task_stack_create(&finished, NULL);

    struct list* group_tasks = NULL;
    list_create(&group_tasks);

    Stack_node* sn = NULL;
    Stack_node_create(&sn);
    sn->mts = mts;
    sn->sl = sl;

    Stack_list_add(sl, sn);

    re_match_add_task(root, slice, mts, NULL, sn);

    return sl;
}

void Remove_finished_task(Match_task_stack* mts, Match_task* task)
{
    if (task->status == Match_task_finished) {
        assert(task == mts->top);
        Match_task_stack_pop(mts);
    }
}

void Cleanup_finished_task(Match_task* task)
{
    if (task->status == Match_task_finished) {
        Match_task_destroy(task);
        free(task);
    }
}

void Check_if_done(Match_task_stack* mts, Match_task* task, bool* done, bool* matched)
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

void Get_groups(Stack_node* sn, struct buffer_list* groups)
{
    size_t index = 0;
    while (true) {
        struct buffer* bf = Hash_map_size_t_get(&sn->groups, index);
        if (!bf) break;
        buffer_list_add_bf(groups, bf);
        index++;
    }
}

Match_task* re_match_add_task(
    Ast_node* n,
    String_slice slice,
    Match_task_stack* mts,
    Match_task* parent,
    Stack_node* sn)
{
    Match_task* task = NULL;
    Match_task_create(&task, parent);
    task->n = n;
    task->start_slice = slice;
    Match_task_stack_push(mts, task);
    return task;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_run_dispatch(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    assert(task);
    assert(task->status != Match_task_finished);

    if (task->status == Match_task_initial && task->n) {
        if (task->n->is_root) {
            struct buffer* bf = NULL;
            buffer_create(&bf);
            Hash_map_size_t_add(&sn->groups, task->n->group, bf);
        }

        if (task->n->is_group) {
            struct buffer* bf = NULL;
            buffer_create(&bf);
            Hash_map_size_t_add(&sn->groups, task->n->group, bf);
        }
    }

    if (!task->n) {
        if (task->start_slice.size == 0) {
            task->matched = true;
        }
        task->status = Match_task_finished;
        struct buffer* bf = NULL;
        buffer_create(&bf);
        Hash_map_size_t_add(&sn->groups, 0, bf);
    } else if (task->n->type == Ast_type_union) {
        re_match_run_union(sn);
    } else if (task->n->type == Ast_type_concat) {
        re_match_run_concat(sn);
    } else if (task->n->type == Ast_type_closure) {
        re_match_run_closure(sn);
    } else if (task->n->type == Ast_type_positive_closure) {
        re_match_run_pos_closure(sn);
    } else if (task->n->type == Ast_type_repeat) {
        re_match_run_repeat(sn);
    } else if (task->n->type == Ast_type_repeat_range) {
        re_match_run_repeat_range(sn);
    } else if (task->n->type == Ast_type_group) {
        re_match_run_group(sn);
    } else if (task->n->type == Ast_type_option) {
        re_match_run_option(sn);
    } else if (task->n->type == Ast_type_literal) {
        re_match_run_literal(sn);
    } else if (task->n->type == Ast_type_wildcard) {
        re_match_run_wildcard(sn);
    } else if (task->n->type == Ast_type_begin) {
        re_match_run_begin(sn);
    } else if (task->n->type == Ast_type_end) {
        re_match_run_end(sn);
    } else if (task->n->type == Ast_type_escape) {
        re_match_run_escape(sn);
    } else if (task->n->type == Ast_type_character_class) {
        re_match_run_character_class(sn);
    } else if (task->n->type == Ast_type_character_class_opposite) {
        re_match_run_character_class_opposite(sn);
    } else if (task->n->type == Ast_type_character_range) {
        re_match_run_character_range(sn);
    } else if (task->n->type == Ast_type_character_type_word) {
        re_match_run_character_type_word(sn, false);
    } else if (task->n->type == Ast_type_character_type_word_opposite) {
        re_match_run_character_type_word(sn, true);
    } else if (task->n->type == Ast_type_character_type_digit) {
        re_match_run_character_type_digit(sn, false);
    } else if (task->n->type == Ast_type_character_type_digit_opposite) {
        re_match_run_character_type_digit(sn, true);
    } else if (task->n->type == Ast_type_character_type_space) {
        re_match_run_character_type_space(sn, false);
    } else if (task->n->type == Ast_type_character_type_space_opposite) {
        re_match_run_character_type_space(sn, true);
    } else {
        assert(false && "Not implemented");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_dispatch(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (parent) {
        if (parent->n) {
            if (parent->n->type == Ast_type_union) {
                re_match_child_finish_union(sn, parent, child);
            } else if (parent->n->type == Ast_type_concat) {
                re_match_child_finish_concat(sn, parent, child);
            } else if (parent->n->type == Ast_type_closure) {
                re_match_child_finish_closure(sn, parent, child);
            } else if (parent->n->type == Ast_type_positive_closure) {
                re_match_child_finish_pos_closure(sn, parent, child);
            } else if (parent->n->type == Ast_type_repeat) {
                re_match_child_finish_repeat(sn, parent, child);
            } else if (parent->n->type == Ast_type_repeat_range) {
                re_match_child_finish_repeat_range(sn, parent, child);
            } else if (parent->n->type == Ast_type_group) {
                re_match_child_finish_group(sn, parent, child);
            } else if (parent->n->type == Ast_type_option) {
                re_match_child_finish_option(sn, parent, child);
            } else if (parent->n->type == Ast_type_literal) {
                assert(false && "literal tasks should not have child tasks");
            } else if (parent->n->type == Ast_type_wildcard) {
                assert(false && "wildcard tasks should not have child tasks");
            } else if (parent->n->type == Ast_type_begin) {
                assert(false && "begin tasks should not have child tasks");
            } else if (parent->n->type == Ast_type_end) {
                assert(false && "end tasks should not have child tasks");
            } else if (parent->n->type == Ast_type_escape) {
                assert(false && "escape tasks should not have child tasks");
            } else if (parent->n->type == Ast_type_character_class) {
                re_match_child_finish_character_class(sn, parent, child);
            } else if (parent->n->type == Ast_type_character_class_opposite) {
                re_match_child_finish_character_class_opposite(sn, parent, child);
            } else if (parent->n->type == Ast_type_character_range) {
                assert(false && "character range task should not have child tasks");
            } else if (parent->n->type == Ast_type_character_type_word) {
                assert(false && "character type word task should not have child tasks");
            } else if (parent->n->type == Ast_type_character_type_digit) {
                assert(false && "character type digit task should not have child tasks");
            } else if (parent->n->type == Ast_type_character_type_space) {
                assert(false && "character type space task should not have child tasks");
            } else {
                assert(false && "invalid type");
            }
        }
    }
}

void re_match_run_union(Stack_node* sn)
{
    Match_task* task = sn->mts->top;

    if (task->status == Match_task_initial) {
        task->p = task->n->head;
        assert(task->p);
        task->status = Match_task_started;
        task->end_slice = task->start_slice;
        task->matched = false;

        Ast_node* p = task->p->next;
        while (p) {
            Stack_node* new_sn = Stack_node_clone(sn);
            new_sn->mts->top->p = p;
            Stack_list_add(sn->sl, new_sn);
            re_match_add_task(
                new_sn->mts->top->p,
                new_sn->mts->top->start_slice,
                new_sn->mts,
                new_sn->mts->top,
                new_sn);
            p = p->next;
        }

        re_match_add_task(task->p, task->start_slice, sn->mts, task, sn);
    } else {
        assert(false && "should not happen");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_union(Stack_node* sn, Match_task* parent, Match_task* child)
{
    parent->matched = child->matched;
    parent->status = Match_task_finished;
    if (child->matched) {
        parent->end_slice = child->end_slice;
    }
    re_match_child_finish_dispatch(sn, parent->parent, parent);
}

void re_match_run_concat(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    if (task->status == Match_task_initial) {
        task->p = task->n->head;
        task->status = Match_task_started;
        task->end_slice = task->start_slice;
        task->matched = true;
    }

    if (task->p) {
        task->status = Match_task_started;
        re_match_add_task(task->p, task->end_slice, sn->mts, task, sn);
        task->p = task->p->next;
    } else {
        task->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_concat(Stack_node* sn, Match_task* parent, Match_task* child)
{
    parent->matched = parent->matched && child->matched;
    if (child->matched) {
        parent->end_slice = child->end_slice;
    } else {
        parent->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void re_match_run_closure(Stack_node* sn)
{
    Match_task *task = sn->mts->top;

    if (task->status == Match_task_initial) {
        task->end_slice = task->start_slice;
        task->status = Match_task_started;
        task->matched = true;
    }

    if (task->status != Match_task_finished) {
        Stack_node* new_sn = Stack_node_clone(sn);
        Match_task* new_task = new_sn->mts->top;
        re_match_add_task(new_task->n->head, new_task->end_slice, new_sn->mts, new_task, new_sn);
        new_sn->priority = Stack_list_next_priority(new_sn->sl);
        Stack_list_add(sn->sl, new_sn);

        task->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_closure(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
    } else {
        parent->matched = false;
        parent->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void re_match_run_pos_closure(Stack_node* sn)
{
    Match_task *task = sn->mts->top;

    if (task->status == Match_task_initial) {
        task->end_slice = task->start_slice;
        task->status = Match_task_started;

        re_match_add_task(task->n->head, task->end_slice, sn->mts, task, sn);
    } else {
        Stack_node* new_sn = Stack_node_clone(sn);
        Match_task* new_task = new_sn->mts->top;
        re_match_add_task(new_task->n->head, new_task->end_slice, new_sn->mts, new_task, new_sn);
        Stack_list_add(sn->sl, new_sn);

        task->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_pos_closure(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->matched = true;
    } else {
        parent->status = Match_task_finished;
        // parent->matched = true;
        re_match_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void re_match_run_repeat(Stack_node* sn)
{
    Match_task *task = sn->mts->top;
    if (task->status == Match_task_initial) {
        task->end_slice = task->start_slice;
        task->status = Match_task_started;
    }

    Ast_node* child_node = task->n->head;
    Ast_node* repeat_node = child_node->next;
    if (repeat_node->num_value == 0) {
        task->status = Match_task_finished;
        task->matched = true;
    }

    if (task->count < repeat_node->num_value) {
        re_match_add_task(child_node, task->end_slice, sn->mts, task, sn);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_repeat(Stack_node* sn, Match_task* parent, Match_task* child)
{
    Ast_node* child_node = parent->n->head;
    Ast_node* repeat_node = child_node->next;

    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->count++;
        if (repeat_node->num_value == parent->count) {
            parent->matched = true;
            parent->status = Match_task_finished;
        }
    } else {
        parent->status = Match_task_finished;
    }

    re_match_child_finish_dispatch(sn, parent->parent, parent);
}

void re_match_run_repeat_range(Stack_node* sn)
{
    Match_task *task = sn->mts->top;
    if (task->status == Match_task_initial) {
        task->end_slice = task->start_slice;
        task->status = Match_task_started;
    }

    Ast_node* child_node = task->n->head;
    Ast_node* repeat_low = child_node->next;
    Ast_node* repeat_high = repeat_low->next;

    if (task->count >= repeat_low->num_value && task->count <= repeat_high->num_value) {
        Stack_node* new_sn = Stack_node_clone(sn);
        Match_task* new_task = new_sn->mts->top;
        re_match_add_task(new_task->n->head, new_task->end_slice, new_sn->mts, new_task, new_sn);
        Stack_list_add(sn->sl, new_sn);

        task->status = Match_task_finished;
        task->matched = true;
        re_match_child_finish_dispatch(sn, task->parent, task);
    } else if (task->count < repeat_high->num_value) {
        re_match_add_task(child_node, task->end_slice, sn->mts, task, sn);
    } else {
        task->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, task->parent, task);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_repeat_range(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->count++;
    } else {
        parent->status = Match_task_finished;
    }

    re_match_child_finish_dispatch(sn, parent->parent, parent);
}

void re_match_run_group(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    if (task->n->head) {
        re_match_add_task(task->n->head, task->start_slice, sn->mts, task, sn);
        task->status = Match_task_started;
    } else {
        task->status = Match_task_finished;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_group(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (child->matched) {
        parent->matched = true;
        parent->end_slice = child->end_slice;
    }
    parent->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, parent->parent, parent);
}

void re_match_run_option(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    if (task->status == Match_task_initial) {
        task->end_slice = task->start_slice;
        task->status = Match_task_started;
    }

    if (task->count == 0) {
        Stack_node* new_sn = Stack_node_clone(sn);
        Match_task* new_task = new_sn->mts->top;
        re_match_add_task(new_task->n->head, new_task->end_slice, new_sn->mts, new_task, new_sn);
        Stack_list_add(sn->sl, new_sn);

        task->status = Match_task_finished;
        task->matched = true;
        re_match_child_finish_dispatch(sn, task->parent, task);
    } else if (task->count == 1) {
        task->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, task->parent, task);
    } else {
        assert(false && "option count should be 0 or 1");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_option(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (child->matched) {
        parent->end_slice = child->end_slice;
        parent->count++;
        parent->matched = true;
    } else {
        parent->status = Match_task_finished;
        re_match_child_finish_dispatch(sn, parent->parent, parent);
    }
}

void Increment_slice(String_slice* slice)
{
    int num = NUM_BYTES(slice->p[0]);
    slice->p += num;
    slice->size -= num;
}

void re_match_run_literal(Stack_node* sn)
{
    Match_task* task = sn->mts->top;

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
                    Match_task_stack_add_char(sn, task, slice.p[0]);
                    Increment_slice(&slice);
                }
            }
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

void re_match_run_wildcard(Stack_node* sn)
{
    Match_task* task = sn->mts->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        int num = NUM_BYTES(slice.p[0]);
        if (num != 1 || slice.p[0] != '\n') {
            for (int i = 0; i < num; i++) {
                Match_task_stack_add_char(sn, task, slice.p[0]);
            }
            Increment_slice(&slice);
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

void re_match_run_begin(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    task->end_slice = task->start_slice;
    if (task->end_slice.p == sn->sl->slice.p) {
        task->matched = true;
    }
    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

void re_match_run_end(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    task->end_slice = task->start_slice;

    if (task->end_slice.size == 0) {
        task->matched = true;
    } else if (task->end_slice.p[0] == '\n') {
        task->matched = true;
        Match_task_stack_add_char(sn, task, '\n');
        Increment_slice(&task->end_slice);
    }
    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

void re_match_run_escape(Stack_node* sn)
{
    Match_task* task = sn->mts->top;

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
                    Match_task_stack_add_char(sn, task, slice.p[0]);
                    Increment_slice(&slice);
                }
            }
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_run_character_class(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    if (task->status == Match_task_initial) {
        task->p = task->n->head;
        task->status = Match_task_started;
        task->end_slice = task->start_slice;
    }

    Ast_node* p = task->n->head;
    while (p && !task->matched) {
        re_match_add_task(p, task->start_slice, sn->mts, task, sn);
        re_match_run_dispatch(sn);
        Remove_finished_task(sn->mts, sn->mts->top);
        p = p->next;
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_character_class(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (child->matched) {
        parent->matched = true;
        parent->end_slice = child->end_slice;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_run_character_class_opposite(Stack_node* sn)
{
    Match_task* task = sn->mts->top;
    if (task->status == Match_task_initial) {
        task->p = task->n->head;
        task->status = Match_task_started;
        task->end_slice = task->start_slice;
        task->matched = true;
    }

    Ast_node* p = task->n->head;
    while (p && task->matched) {
        Match_task* child = re_match_add_task(p, task->start_slice, sn->mts, task, sn);
        child->opposite = true;
        re_match_run_dispatch(sn);
        Remove_finished_task(sn->mts, sn->mts->top);
        p = p->next;
    }

    if (task->matched) {
        Match_task_stack_add_char(sn, task, task->end_slice.p[0]);
        Increment_slice(&task->end_slice);
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void re_match_child_finish_character_class_opposite(Stack_node* sn, Match_task* parent, Match_task* child)
{
    if (!child->matched) {
        parent->matched = false;
        parent->end_slice = child->end_slice;
    }
}

void re_match_run_character_range(Stack_node* sn)
{
    Match_task* task = sn->mts->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        Ast_node* a = task->n->head;
        Ast_node* b = a->next;
        bool matched = IS_ONE_BYTE(a->c[0]) && IS_ONE_BYTE(b->c[0]) && IS_ONE_BYTE(slice.p[0])
            && slice.p[0] >= a->c[0] && slice.p[0] <= b->c[0];
        if (task->opposite) {
            matched = !matched;
        }
        if (matched) {
            if (!task->opposite) {
                Match_task_stack_add_char(sn, task, slice.p[0]);
                Increment_slice(&slice);
            }
            task->matched = true;
            task->end_slice = slice;
        }
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

void re_match_run_character_type_word(Stack_node* sn, bool opposite)
{
    Match_task* task = sn->mts->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        bool is_word = IS_ONE_BYTE(slice.p[0])
            && (isalpha(slice.p[0]) || isdigit(slice.p[0]) || slice.p[0] == '_');
        if ((!opposite && is_word) || (opposite && !is_word)) {
            task->matched = true;
            Match_task_stack_add_char(sn, task, slice.p[0]);
            Increment_slice(&slice);
            task->end_slice = slice;
        }
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

void re_match_run_character_type_digit(Stack_node* sn, bool opposite)
{
    Match_task* task = sn->mts->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        bool is_digit = IS_ONE_BYTE(slice.p[0]) && isdigit(slice.p[0]);
        if ((is_digit && !opposite) || (!is_digit && opposite)) {
            task->matched = true;
            Match_task_stack_add_char(sn, task, slice.p[0]);
            Increment_slice(&slice);
            task->end_slice = slice;
        }
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}

void re_match_run_character_type_space(Stack_node* sn, bool opposite)
{
    Match_task* task = sn->mts->top;

    if (task->start_slice.size > 0) {
        String_slice slice = task->start_slice;
        bool is_space = IS_ONE_BYTE(slice.p[0]) && isspace(slice.p[0]);
        if ((is_space && !opposite) || (!is_space && opposite)) {
            task->matched = true;
            Match_task_stack_add_char(sn, task, slice.p[0]);
            Increment_slice(&slice);
            task->end_slice = slice;
        }
    }

    task->status = Match_task_finished;
    re_match_child_finish_dispatch(sn, task->parent, task);
}