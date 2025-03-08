#include "module_finder.h"
#include "zinc/os.h"

Cent_input_data Cent_module_finder_find(void* obj, Cent_module_finder_vtable *vtable, Zinc_string* name)
{
#if IS_UNIX
    Cent_module_finder_find_interface* find = obj + vtable->find_offset;
#elif IS_WIN
    Cent_module_finder_find_interface* find = (uint8_t*)obj + vtable->find_offset;
#else #error Unsupported platform
#endif
    return (*find)(obj, name);
}

void Cent_module_finder_destroy(void* obj, Cent_module_finder_vtable *vtable)
{
#if IS_UNIX
    Cent_module_finder_destroy_interface* destroy = obj + vtable->destroy_offset;
#elif IS_WIN
    Cent_module_finder_destroy_interface* destroy = (uint8_t*)obj + vtable->destroy_offset;
#else #error Unsupported platform
#endif
    (*destroy)(obj);
}