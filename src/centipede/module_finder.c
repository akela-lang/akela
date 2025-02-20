#include "module_finder.h"

Cent_input_data Cent_module_finder_find(void* obj, Cent_module_finder_vtable *vtable, Zinc_string* name)
{
    Cent_module_finder_find_interface* find = obj + vtable->find_offset;
    return (*find)(obj, name);
}

void Cent_module_finder_destroy(void* obj, Cent_module_finder_vtable *vtable)
{
    Cent_module_finder_destroy_interface* destroy = obj + vtable->destroy_offset;
    (*destroy)(obj);
}