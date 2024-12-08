#include "match_tools.h"
#include "zinc/memory.h"
#include <assert.h>
#include "zinc/hash.h"
#include "zinc/buffer_list.h"

void Match_task_init(Match_task* task, Match_task* parent)
{
    task->status = Match_task_initial;
    task->matched = false;
    task->n = NULL;
    task->p = NULL;
    task->count = 0;
    task->start_slice.p = NULL;
    task->start_slice.size = 0;
    task->end_slice.p = NULL;
    task->end_slice.size = 0;
    buffer_init(&task->ms);
    task->parent = parent;
    task->next = NULL;
    task->prev = NULL;
}

void Match_task_destroy(Match_task* task)
{
    buffer_destroy(&task->ms);
}

void Match_task_create(Match_task** task, Match_task* parent)
{
    malloc_safe((void**)task, sizeof(Match_task));
    Match_task_init(*task, parent);
}

void Match_task_copy(Match_task* src, Match_task* dest)
{
    dest->status = src->status;
    dest->matched = src->matched;
    dest->n = src->n;
    dest->p = src->p;
    dest->count = src->count;
    dest->start_slice = src->start_slice;
    dest->end_slice = src->end_slice;
    buffer_copy(&src->ms, &dest->ms);
}

void Match_task_stack_init(Match_task_stack* mts, Stack_node* sn)
{
    mts->top = NULL;
    mts->bottom = NULL;
}

void Match_task_stack_create(Match_task_stack** mts, Stack_node* sn)
{
    malloc_safe((void**)mts, sizeof(Match_task_stack));
    Match_task_stack_init(*mts, sn);
}

void Match_task_stack_destroy(Match_task_stack* mts)
{
    if (mts) {
        Match_task* task = mts->top;

        while (task) {
            Match_task* temp = task;
            task = task->next;
            Match_task_destroy(temp);
            free(temp);
        }

        free(mts);
    }
}

void Match_task_stack_push(Match_task_stack *mts, Match_task *new_task)
{
    assert(new_task);
    assert(new_task->next == NULL);
    assert(new_task->prev == NULL);

    if (mts->top && mts->bottom) {
        new_task->next = mts->top;
        mts->top->prev = new_task;
        mts->top = new_task;
    } else {
        mts->top = new_task;
        mts->bottom = new_task;
    }
}

Match_task* Match_task_stack_pop(Match_task_stack* mts)
{
    if (mts->top && mts->bottom) {
        Match_task* task = mts->top;
        mts->top = task->next;
        if (mts->top) {
            mts->top->prev = NULL;
        } else {
            mts->bottom = NULL;
        }
        return task;
    } else {
        return NULL;
    }
}

Match_task* Match_task_stack_remove(Match_task_stack* mts, Match_task* task)
{
    Match_task* prev = task->prev;
    Match_task* next = task->next;
    if (prev) {
        prev->next = next;
    } else {
        mts->bottom = next;
    }
    if (next) {
        next->prev = prev;
    } else {
        mts->top = prev;
    }

    task->prev = NULL;
    task->next = NULL;
    return task;
}

void Match_task_stack_pop_to(Match_task_stack* mts, Match_task* marker)
{
    Match_task* task = mts->top;
    while (task && task != marker) {
        Match_task* temp = task;
        task = task->next;
        Match_task_stack_pop(mts);
        Match_task_destroy(temp);
        free(temp);
    }
}

void Match_task_stack_add_char(Stack_node* sn, Match_task* task, char c)
{
    while (task) {
        buffer_add_char(&task->ms, c);

        if (task->n->is_root) {
            struct buffer* bf = Hash_map_size_t_get(&sn->groups, 0);
            if (!bf) {
                buffer_create(&bf);
                buffer_add_char(bf, c);
                Hash_map_size_t_add(&sn->groups, 0, bf);
            } else {
                buffer_add_char(bf, c);
            }
        }

        if (task->n->is_group) {
            struct buffer* bf = Hash_map_size_t_get(&sn->groups, task->n->group);
            if (!bf) {
                buffer_create(&bf);
                buffer_add_char(bf, c);
                Hash_map_size_t_add(&sn->groups, task->n->group, bf);
            } else {
                buffer_add_char(bf, c);
            }
        }

        task = task->parent;
    }
}

Match_task_stack* Match_task_stack_clone(Match_task_stack* mts, struct hash_table* ht, Stack_node* sn)
{
    if (mts) {
        Match_task_stack* new_mts = NULL;
        Match_task_stack_create(&new_mts, sn);

        struct buffer bf;
        buffer_init(&bf);

        Match_task* task = mts->bottom;
        while (task) {
            Match_task* new_task = NULL;
            Match_task_create(&new_task, NULL);
            Match_task_copy(task, new_task);
            Match_task_stack_push(new_mts, new_task);

            buffer_clear(&bf);
            buffer_add_format(&bf, "%lx", task);
            hash_table_add(ht, &bf, new_task);

            if (task->parent) {
                buffer_clear(&bf);
                buffer_add_format(&bf, "%lx", task->parent);
                new_task->parent = hash_table_get(ht, &bf);
            }

            task = task->prev;
        }


        buffer_destroy(&bf);

        return new_mts;
    }

    return NULL;
}

void Stack_node_init(Stack_node* sn)
{
    sn->mts = NULL;
    sn->sl = NULL;
    Hash_map_size_t_init(&sn->groups, 16);
    sn->priority = 0;
    sn->next = NULL;
    sn->prev = NULL;
}

void Stack_node_create(Stack_node** sn)
{
    malloc_safe((void**)sn, sizeof(Stack_node));
    Stack_node_init(*sn);
}

void Stack_node_groups_item_destroy(size_t index, struct buffer* bf)
{
    buffer_destroy(bf);
    free(bf);
}

void Stack_node_destroy(Stack_node* sn)
{
    Match_task_stack_destroy(sn->mts);
    Hash_map_size_t_map(&sn->groups, (Hash_map_size_t_func)Stack_node_groups_item_destroy);
    Hash_map_size_t_destroy(&sn->groups);
    free(sn);
}

Hash_map_size_t* Stack_target_hash = NULL;
void Stack_node_group_copy(size_t index, struct buffer* bf)
{
    struct buffer* bf2 = buffer_clone(bf);
    Hash_map_size_t_add(Stack_target_hash, index, bf2);
}

Stack_node* Stack_node_clone(Stack_node* sn)
{
    struct hash_table ht;
    hash_table_init(&ht, 32);

    Stack_node* new_sn = NULL;
    Stack_node_create(&new_sn);

    new_sn->mts = Match_task_stack_clone(sn->mts, &ht, new_sn);
    new_sn->sl = sn->sl;

    Stack_target_hash = &new_sn->groups;
    Hash_map_size_t_map(&sn->groups, (Hash_map_size_t_func)Stack_node_group_copy);

    hash_table_destroy(&ht);

    return new_sn;
}

void Stack_node_dump_group_buffer(size_t index, struct buffer* bf)
{
    buffer_finish(bf);
    printf("%zu: %s\n", index, bf->buf);
}

void Stack_node_dump_groups(Stack_node* sn)
{
    Hash_map_size_t_map(&sn->groups, (Hash_map_size_t_func)Stack_node_dump_group_buffer);
}

void Stack_list_init(Stack_list* sl)
{
    sl->head = NULL;
    sl->tail = NULL;
    sl->slice.p = NULL;
    sl->slice.size = 0;
    sl->top_priority = 0;
}

void Stack_list_create(Stack_list** sl)
{
    malloc_safe((void**)sl, sizeof(Stack_list));
    Stack_list_init(*sl);
}

Stack_node* Stack_list_add(Stack_list* sl, Stack_node* sn)
{
    if (sl->head && sl->tail) {
        Stack_node* p = sl->tail;
        while (p && p->priority > sn->priority) {
            p = p->next;
        }
        if (p) {
            if (p->next) {
                sn->next = p->next;
            } else {
                sl->tail = sn;
            }
            sn->prev = p;
            p->next = sn;
        } else {
            sn->next = sl->head;
            sl->head->prev = sn;
            sl->head = sn;
        }
    } else {
        sl->head = sn;
        sl->tail = sn;
    }

    return sn;
}

size_t Stack_list_next_priority(Stack_list* sl)
{
    return ++sl->top_priority;
}

void Stack_list_remove(Stack_list* sl, Stack_node* sn)
{
    if (sn->prev) {
        sn->prev->next = sn->next;
    }

    if (sn->next) {
        sn->next->prev = sn->prev;
    }

    if (sl->head == sn) {
        sl->head = sn->next;
    }

    if (sl->tail == sn) {
        sl->tail = sn->prev;
    }

    sn->next = NULL;
    sn->prev = NULL;
}

void Stack_list_destroy(Stack_list* sl)
{
    Stack_node* sn = sl->head;
    while (sn) {
        Stack_node* temp = sn;
        sn = sn->next;
        Stack_node_destroy(temp);
    }

    free(sl);
}