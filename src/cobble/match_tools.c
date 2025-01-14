#include "match_tools.h"
#include "zinc/memory.h"
#include <assert.h>
#include "zinc/hash.h"
#include "zinc/buffer_list.h"
#include "zinc/result.h"

#include <unicode/uchar.h>
#include <unicode/locid.h>
#include <unicode/stringoptions.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/ustring.h>
#include <unicode/ucnv.h>
#include <zinc/utf8.h>

void Cob_task_init(Cob_task* task, Cob_task* parent)
{
    task->status = Cob_task_status_initial;
    task->matched = false;
    task->n = NULL;
    task->p = NULL;
    task->count = 0;
    task->start_slice.p = NULL;
    task->start_slice.size = 0;
    task->end_slice.p = NULL;
    task->end_slice.size = 0;
    task->opposite = false;
    task->parent = parent;
    task->next = NULL;
    task->prev = NULL;
}

void Cob_task_create(Cob_task** task, Cob_task* parent)
{
    malloc_safe((void**)task, sizeof(Cob_task));
    Cob_task_init(*task, parent);
}

void Cob_task_copy(Cob_task* src, Cob_task* dest)
{
    dest->status = src->status;
    dest->matched = src->matched;
    dest->n = src->n;
    dest->p = src->p;
    dest->count = src->count;
    dest->start_slice = src->start_slice;
    dest->end_slice = src->end_slice;
}

void Cob_stack_init(Cob_stack* mts, Cob_stack_node* sn)
{
    mts->top = NULL;
    mts->bottom = NULL;
}

void Cob_stack_create(Cob_stack** mts, Cob_stack_node* sn)
{
    malloc_safe((void**)mts, sizeof(Cob_stack));
    Cob_stack_init(*mts, sn);
}

void Cob_stack_destroy(Cob_stack* mts)
{
    if (mts) {
        Cob_task* task = mts->top;

        while (task) {
            Cob_task* temp = task;
            task = task->next;
            free(temp);
        }

        free(mts);
    }
}

void Cob_stack_push(Cob_stack *mts, Cob_task *new_task)
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

Cob_task* Cob_stack_pop(Cob_stack* mts)
{
    if (mts->top && mts->bottom) {
        Cob_task* task = mts->top;
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

Cob_task* Cob_stack_remove(Cob_stack* mts, Cob_task* task)
{
    Cob_task* prev = task->prev;
    Cob_task* next = task->next;
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

void Cob_stack_pop_to(Cob_stack* mts, Cob_task* marker)
{
    Cob_task* task = mts->top;
    while (task && task != marker) {
        Cob_task* temp = task;
        task = task->next;
        Cob_stack_pop(mts);
        free(temp);
    }
}

void Cob_stack_node_add_char(Cob_stack_node* sn, Cob_task* task, String_slice slice)
{
    int num = NUM_BYTES(slice.p[0]);
    while (task) {
        if (task->n->is_root) {
            struct Zinc_string* bf = Hash_map_size_t_get(&sn->groups, 0);
            if (!bf) {
                Zinc_string_create(&bf);
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(bf, slice.p[i]);
                }
                Hash_map_size_t_add(&sn->groups, 0, bf);
            } else {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(bf, slice.p[i]);
                }
            }
        }

        if (task->n->is_group) {
            struct Zinc_string* bf = Hash_map_size_t_get(&sn->groups, task->n->group);
            if (!bf) {
                Zinc_string_create(&bf);
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(bf, slice.p[i]);
                }
                Hash_map_size_t_add(&sn->groups, task->n->group, bf);
            } else {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(bf, slice.p[i]);
                }
            }
        }

        task = task->parent;
    }
}

Cob_stack* Cob_stack_clone(Cob_stack* mts, struct hash_table* ht, Cob_stack_node* sn)
{
    if (mts) {
        Cob_stack* new_mts = NULL;
        Cob_stack_create(&new_mts, sn);

        struct Zinc_string bf;
        Zinc_string_init(&bf);

        Cob_task* task = mts->bottom;
        while (task) {
            Cob_task* new_task = NULL;
            Cob_task_create(&new_task, NULL);
            Cob_task_copy(task, new_task);
            Cob_stack_push(new_mts, new_task);

            Zinc_string_clear(&bf);
            Zinc_string_add_format(&bf, "%lx", task);
            hash_table_add(ht, &bf, new_task);

            if (task->parent) {
                Zinc_string_clear(&bf);
                Zinc_string_add_format(&bf, "%lx", task->parent);
                new_task->parent = hash_table_get(ht, &bf);
            }

            task = task->prev;
        }


        Zinc_string_destroy(&bf);

        return new_mts;
    }

    return NULL;
}

void Cob_stack_node_init(Cob_stack_node* sn)
{
    sn->stack = NULL;
    sn->sl = NULL;
    Hash_map_size_t_init(&sn->groups, 16);
    sn->priority = 0;
    sn->next = NULL;
    sn->prev = NULL;
}

void Cob_stack_node_create(Cob_stack_node** sn)
{
    malloc_safe((void**)sn, sizeof(Cob_stack_node));
    Cob_stack_node_init(*sn);
}

void Cob_stack_node_groups_item_destroy(size_t index, struct Zinc_string* bf)
{
    Zinc_string_destroy(bf);
    free(bf);
}

void Cob_stack_node_destroy(Cob_stack_node* sn)
{
    Cob_stack_destroy(sn->stack);
    Hash_map_size_t_map(&sn->groups, (Hash_map_size_t_func)Cob_stack_node_groups_item_destroy);
    Hash_map_size_t_destroy(&sn->groups);
    free(sn);
}

Hash_map_size_t* Stack_target_hash = NULL;
void Cob_stack_node_group_copy(size_t index, struct Zinc_string* bf)
{
    struct Zinc_string* bf2 = Zinc_string_clone(bf);
    Hash_map_size_t_add(Stack_target_hash, index, bf2);
}

Cob_stack_node* Cob_stack_node_clone(Cob_stack_node* sn)
{
    struct hash_table ht;
    hash_table_init(&ht, 32);

    Cob_stack_node* new_sn = NULL;
    Cob_stack_node_create(&new_sn);

    new_sn->stack = Cob_stack_clone(sn->stack, &ht, new_sn);
    new_sn->sl = sn->sl;

    Stack_target_hash = &new_sn->groups;
    Hash_map_size_t_map(&sn->groups, (Hash_map_size_t_func)Cob_stack_node_group_copy);

    hash_table_destroy(&ht);

    return new_sn;
}

void Cob_stack_node_dump_group_buffer(size_t index, struct Zinc_string* bf)
{
    Zinc_string_finish(bf);
    printf("%zu: %s\n", index, bf->buf);
}

void Cob_stack_node_dump_groups(Cob_stack_node* sn)
{
    Hash_map_size_t_map(&sn->groups, (Hash_map_size_t_func)Cob_stack_node_dump_group_buffer);
}

void Cob_stack_list_init(Cob_stack_list* sl)
{
    sl->head = NULL;
    sl->tail = NULL;
    sl->slice.p = NULL;
    sl->slice.size = 0;
    sl->top_priority = 0;
}

void Cob_stack_list_create(Cob_stack_list** sl)
{
    malloc_safe((void**)sl, sizeof(Cob_stack_list));
    Cob_stack_list_init(*sl);
}

Cob_stack_node* Cob_stack_list_add(Cob_stack_list* sl, Cob_stack_node* sn)
{
    if (sl->head && sl->tail) {
        Cob_stack_node* p = sl->tail;
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

size_t Cob_stack_list_next_priority(Cob_stack_list* sl)
{
    return ++sl->top_priority;
}

void Cob_stack_list_remove(Cob_stack_list* sl, Cob_stack_node* sn)
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

void Cob_stack_list_destroy(Cob_stack_list* sl)
{
    Cob_stack_node* sn = sl->head;
    while (sn) {
        Cob_stack_node* temp = sn;
        sn = sn->next;
        Cob_stack_node_destroy(temp);
    }

    free(sl);
}

void Cob_result_init(Cob_result* mr)
{
    mr->matched = false;
    Zinc_string_list_init(&mr->groups);
}

void Cob_result_destroy(Cob_result* mr)
{
    Zinc_string_list_destroy(&mr->groups);
}