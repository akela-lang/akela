#ifndef CENTIPEDE_MODULE_FILE_H
#define CENTIPEDE_MODULE_FILE_H

#include "module_finder.h"

typedef struct Cent_module_finder_file {
    Zinc_string dir_path;
    Cent_module_finder_find_interface find;
    Cent_module_finder_destroy_interface destroy;
    Cent_module_finder_vtable* vtable;
} Cent_module_finder_file;

extern Cent_module_finder_vtable Cent_module_finder_file_vtable;

void Cent_module_finder_file_init(Cent_module_finder_file *mf, Zinc_string* dir_path);
void Cent_module_finder_file_create(Cent_module_finder_file** mf, Zinc_string* dir_path);
void Cent_module_finder_file_destroy(Cent_module_finder_file* mf);
void Cent_module_finder_file_append_path(Zinc_string* path1, Zinc_string* path2);
void Cent_module_finder_file_append_path(Zinc_string* path1, Zinc_string* path2);
Cent_input_data Cent_module_finder_file_find(Cent_module_finder_file* mf, Zinc_string* name);

#endif
