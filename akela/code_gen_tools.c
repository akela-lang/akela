#include "code_gen_tools.h"

void cg_data_init(struct cg_data* cgd, LLVMBuilderRef builder, LLVMContextRef context)
{
    cgd->builder = builder;
    cgd->context = context;
    error_list_init(&cgd->el);
    hash_table_init(&cgd->named_values, NAMED_VALUES_HASH_SIZE);
}

void cg_data_destroy(struct cg_data* cgd)
{
    error_list_destroy(&cgd->el);
    hash_table_destroy(&cgd->named_values);
}
