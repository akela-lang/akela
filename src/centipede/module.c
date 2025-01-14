#include "module.h"

Cent_input_data Cent_module_find_interface(void* obj, Cent_module_vtable *vtable, struct Zinc_string* name)
{
    Cent_module_find* find = obj + vtable->find_offset;
    return (*find)(obj, name);
}