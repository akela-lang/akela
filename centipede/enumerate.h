#ifndef CENTIPEDE_ENUMERATE_H
#define CENTIPEDE_ENUMERATE_H

#include "zinc/buffer.h"
#include "zinc/buffer_list.h"

typedef struct Cent_enumerate {
    struct buffer name;
    struct buffer_list names;
} Cent_enumerate;

void Cent_enumerate_init(Cent_enumerate* en);
void Cent_enumerate_create(Cent_enumerate** en);
void Cent_enumerate_destroy(Cent_enumerate* en);
void Cent_enumerate_add_name(Cent_enumerate* en, struct buffer* name);
void Cent_enumerate_add_name_str(Cent_enumerate* en, char* name);

#endif