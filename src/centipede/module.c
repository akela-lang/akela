#include "module.h"

Cent_comp_unit* Cent_module_find_interface(void* obj, Cent_module_vtable *vtable, struct buffer* name)
{
    Cent_module_find* find = obj + vtable->find_offset;
    return (*find)(obj, name);
}