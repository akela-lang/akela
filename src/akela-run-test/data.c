#include "data.h"
#include "zinc/memory.h"
#include "cobble/compile_data.h"

void Run_data_init(Run_data* data)
{
    data->ext_re = Cob_compile_str("\\.ake$");
    data->llvm_re = Cob_compile_str("^##(.*\n)");
    data->result_re = Cob_compile_str("^###(.*\n)");
}

void Run_data_create(Run_data** data)
{
    Zinc_malloc_safe((void**)data, sizeof(Run_data));
    Run_data_init(*data);
}

void Run_data_destroy(Run_data* data)
{
    Cob_re_destroy(&data->ext_re);
    Cob_re_destroy(&data->llvm_re);
    Cob_re_destroy(&data->result_re);
}