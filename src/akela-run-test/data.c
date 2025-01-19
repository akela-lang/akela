#include "data.h"
#include "zinc/memory.h"
#include "cobble/compile_data.h"

void Run_data_init(Run_data* data)
{
    data->ext_re = Cob_compile_str("\\.ake$");
    data->llvm_re = Cob_compile_str("^##(.*\n)");
    data->result_re = Cob_compile_str("^###(.*\n)");
    data->test_count = 0;
    data->test_failed_count = 0;
    data->test_passed_count = 0;
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

void Run_pair_init(Run_pair* pair)
{
    pair->matched = true;
    pair->actual = NULL;
    pair->expected = NULL;
}

void Run_pair_create(Run_pair** pair)
{
    Zinc_malloc_safe((void**)pair, sizeof(Run_pair));
    Run_pair_init(*pair);
}

void Run_pair_destroy(Run_pair* pair)
{
    Zinc_string_destroy(pair->actual);
    free(pair->actual);
    Zinc_string_destroy(pair->expected);
    free(pair->expected);
}