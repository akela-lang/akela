#include "module_file.h"
#include <stddef.h>

Cent_module_vtable Cent_module_file_vtable = {
    .find_offset = offsetof(Cent_module_file, find),
};
