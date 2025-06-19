#ifndef AKELA_ENVIRONMENT2_H
#define AKELA_ENVIRONMENT2_H

#include "forward.h"
#include "zinc/zstring.h"
#include "symbol.h"
#include "symbol_table.h"

#define AKE_ENVIRONMENT_SIZE 32
#define AKE_SEQ_DEFAULT 0
#define AKE_SEQ_ANY SIZE_MAX

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Ake_Sequence Ake_Sequence;
struct Ake_Sequence {
    size_t count;
};

typedef struct Ake_EnvironmentEntry Ake_EnvironmentEntry;
struct Ake_EnvironmentEntry {
    Ake_symbol* sym;
    Zinc_string name;
    size_t seq;
    Ake_EnvironmentEntry* next;
    Ake_EnvironmentEntry* prev;
};

typedef struct Ake_EnvironmentEntryList Ake_EnvironmentEntryList;
struct Ake_EnvironmentEntryList {
    Ake_EnvironmentEntry* head;
    Ake_EnvironmentEntry* tail;
};

typedef struct Ake_Environment Ake_Environment;
struct Ake_Environment {
    size_t size;
    Ake_EnvironmentEntryList buckets[AKE_ENVIRONMENT_SIZE];
    Ake_Environment* prev;
    bool is_global;
};

typedef void (*Ake_EnvironmentEntryFunc)(void*);
typedef void (*Ake_EnvironmentEntryNameFunc)(Zinc_string*, void*);

size_t Ake_HashCalcString(Zinc_string* name, size_t size);
size_t Ake_HashCalcStr(char* str, size_t size);

void Ake_EnvironmentEntryInit(Ake_EnvironmentEntry* ent);
void Ake_EnvironmentEntryDestroy(Ake_EnvironmentEntry* ent);

void Ake_EnvironmentEntryListInit(Ake_EnvironmentEntryList* list);
void Ake_EnvironmentEntryListDestroy(Ake_EnvironmentEntryList* list);

void Ake_EnvironmentInit(Ake_Environment* env, Ake_Environment* prev);
void Ake_EnvironmentCreate(Ake_Environment** env, Ake_Environment* prev);
void Ake_EnvironmentDestroy(Ake_Environment* env);
void Ake_EnvironmentMap(Ake_Environment* env, Ake_EnvironmentEntryFunc f);
void Ake_EnvironmentMapName(Ake_Environment* env, Ake_EnvironmentEntryNameFunc f);

void Ake_EnvironmentAdd(Ake_Environment* env, Zinc_string* name, Ake_symbol* sym, size_t seq);
void Ake_EnvironmentAddStr(Ake_Environment* env, char* str, Ake_symbol* sym, size_t seq);
Ake_symbol* Ake_EnvironmentGetLocal(Ake_Environment* env, Zinc_string* name, size_t seq);
Ake_symbol* Ake_EnvironmentGetLocalStr(Ake_Environment* env, char* str, size_t seq);
Ake_symbol* Ake_EnvironmentGet(Ake_Environment* env, Zinc_string* name, size_t seq);
Ake_symbol* Ake_EnvironmentGetStr(Ake_Environment* env, char* str, size_t seq);

#ifdef __cplusplus
}
#endif

#endif
