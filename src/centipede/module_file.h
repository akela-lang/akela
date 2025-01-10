#ifndef CENTIPEDE_MODULE_FILE_H
#define CENTIPEDE_MODULE_FILE_H

#include "module.h"

typedef struct Cent_module_file {
    struct buffer dir_path;
    Cent_module_find find;
} Cent_module_file;

extern Cent_module_vtable Cent_module_file_vtable;

void Cent_module_file_init(Cent_module_file *mf, struct buffer* dir_path);
void Cent_module_file_create(Cent_module_file** mf, struct buffer* dir_path);
void Cent_module_file_destroy(Cent_module_file* mf);
void Cent_module_file_append_path(struct buffer* path1, struct buffer* path2);
void Cent_module_file_append_path(struct buffer* path1, struct buffer* path2);
Cent_comp_unit* Cent_module_file_find(Cent_module_file* mf, struct buffer* name);

#endif
