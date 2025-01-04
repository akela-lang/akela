#ifndef CENTIPEDE_MODULE_FILE_H
#define CENTIPEDE_MODULE_FILE_H

#include "module.h"

typedef struct Cent_module_file {
    Cent_module_find find;
} Cent_module_file;

extern Cent_module_vtable Cent_module_file_vtable;

#endif
