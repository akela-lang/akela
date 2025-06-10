#include <stdlib.h>
#include "environment2.h"
#include "string.h"
#include "zinc/memory.h"

size_t Ake_HashCalcString(Zinc_string* name, size_t size)
{
    unsigned int val = 0;

    for (int i = 0; i < name->size; i++) {
        val += (unsigned int)name->buf[i];
        val %= size;
    }

    return val;
}

size_t Ake_HashCalcStr(char* str, size_t size)
{
    unsigned int val = 0;

    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        val += (size_t)str[i];
        val %= size;
    }

    return val;
}

void Ake_EnvironmentEntryInit(Ake_EnvironmentEntry* ent)
{
    ent->sym = NULL;
    Zinc_string_init(&ent->name);
    ent->seq = 0;
    ent->next = NULL;
    ent->prev = NULL;
}

void Ake_EnvironmentEntryDestroy(Ake_EnvironmentEntry* ent)
{
    Zinc_string_destroy(&ent->name);
    Ake_symbol_destroy(ent->sym);
    free(ent->sym);
}

void Ake_EnvironmentEntryListInit(Ake_EnvironmentEntryList* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Ake_EnvironmentEntryListDestroy(Ake_EnvironmentEntryList* list)
{
    Ake_EnvironmentEntry* p = list->head;
    while (p) {
        Ake_EnvironmentEntry* temp = p;
        p = p->next;
        Ake_EnvironmentEntryDestroy(temp);
        free(temp);
    }
}

void Ake_EnvironmentInit(Ake_Environment* env, Ake_Environment* prev)
{
    env->size = AKE_ENVIRONMENT_SIZE;

    for (int i = 0; i < env->size; i++) {
        Ake_EnvironmentEntryListInit(&env->buckets[i]);
    }

    env->prev = prev;
    env->is_global = false;
}

void Ake_EnvironmentCreate(Ake_Environment** env, Ake_Environment* prev)
{
    Zinc_malloc_safe((void**)env, sizeof(Ake_Environment));
    Ake_EnvironmentInit(*env, prev);
}

void Ake_EnvironmentDestroy(Ake_Environment* env)
{
    if (env) {
        for (int i = 0; i < env->size; i++) {
            Ake_EnvironmentEntryListDestroy(&env->buckets[i]);
        }
    }
}

void Ake_EnvironmentMap(Ake_Environment* env, Ake_EnvironmentEntryFunc f)
{
    for (int i = 0; i < env->size; i++) {
        Ake_EnvironmentEntry* p = env->buckets[i].head;
        while (p) {
            Ake_EnvironmentEntry* temp = p;
            p = p->next;
            f(temp->sym);
        }
    }
}

void Ake_EnvironmentMapName(Ake_Environment* env, Ake_EnvironmentEntryNameFunc f)
{
    for (int i = 0; i < env->size; i++) {
        Ake_EnvironmentEntry* p = env->buckets[i].head;
        while (p) {
            Ake_EnvironmentEntry* temp = p;
            p = p->next;
            f(&temp->name, temp->sym);
        }
    }
}

// seq increases for each creation and use of a symbol
// calc hash to get bucket and add to the end of the bucket
void Ake_EnvironmentAdd(Ake_symbol_table* st, Zinc_string* name, Ake_symbol* sym)
{
    Ake_Environment* env = st->top2;
    size_t val = Ake_HashCalcString(name, env->size);

    Ake_EnvironmentEntry* ent;
    Zinc_malloc_safe((void**)&ent, sizeof(struct Ake_EnvironmentEntry));
    Ake_EnvironmentEntryInit(ent);
    ent->sym = sym;
    ent->seq = Ake_symbol_table_get_seq_num(st);

    Zinc_string_copy(name, &ent->name);

    if (env->buckets[val].head && env->buckets[val].tail) {
        ent->prev = env->buckets[val].tail;
        env->buckets[val].tail->next = ent;
        env->buckets[val].tail = ent;
    } else {
        env->buckets[val].tail = ent;
        env->buckets[val].head = ent;
    }
}

void Ake_EnvironmentAddStr(Ake_symbol_table* st, char* str, Ake_symbol* sym)
{
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_string_add_str(&name, str);
    Ake_EnvironmentAdd(st, &name, sym);
    Zinc_string_destroy(&name);
}

// seq increases for each creation and use of a symbol
// calc hash to get the bucket
// start search at the tail and get the first entry that matches name and has smaller or equal seq
Ake_symbol* Ake_EnvironmentGetLocal(Ake_symbol_table* st, Ake_Environment* env, Zinc_string* name)
{
    Ake_EnvironmentEntry* ent;

    size_t val = Ake_HashCalcString(name, env->size);

    ent = env->buckets[val].tail;
    while (ent) {
        if (Zinc_string_compare(&ent->name, name)) {
            if (ent->seq <= st->count) {
                return ent->sym;
            }
        }
        ent = ent->next;
    }

    return NULL;
}

Ake_symbol* Ake_EnvironmentGetLocalStr(Ake_symbol_table* st, Ake_Environment* env, char* str)
{
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_string_add_str(&name, str);
    Ake_symbol* sym = Ake_EnvironmentGetLocal(st, env, &name);
    Zinc_string_destroy(&name);
    return sym;
}

Ake_symbol* Ake_EnvironmentGet(Ake_symbol_table* st, Zinc_string* name)
{
    Ake_Environment* env = st->top2;
    Ake_Environment* p = env;
    while (p) {
        Ake_symbol* sym = Ake_EnvironmentGetLocal(st, p, name);
        if (sym) {
            return sym;
        }
        p = p->prev;
    }

    return NULL;
}

Ake_symbol* Ake_EnvironmentGetStr(Ake_symbol_table* st, char* str)
{
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_string_add_str(&name, str);
    Ake_symbol* sym = Ake_EnvironmentGet(st, &name);
    Zinc_string_destroy(&name);
    return sym;
}

void Ake_EnvironmentBegin(Ake_symbol_table* st)
{
    Ake_Environment* env = NULL;
    Ake_EnvironmentCreate(&env, st->top2);
    st->top2 = env;
}

void Ake_EnvironmentEnd(Ake_symbol_table* st)
{
    Ake_Environment* env = st->top2;
    st->top2 = env->prev;
    env->prev = st->deactivated2;
    st->deactivated2 = env;
}
