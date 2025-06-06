#include "data.h"
#include "zinc/memory.h"
#include "cobble/compile_data.h"
#include "centipede/module_finder_file.h"

void Art_pair_init(Art_pair* pair)
{
    pair->matched = true;
    pair->actual = NULL;
    pair->expected = NULL;
}

void Art_pair_create(Art_pair** pair)
{
    Zinc_malloc_safe((void**)pair, sizeof(Art_pair));
    Art_pair_init(*pair);
}

void Art_pair_destroy(Art_pair* pair)
{
    Zinc_string_destroy(pair->actual);
    free(pair->actual);
    Zinc_string_destroy(pair->expected);
    free(pair->expected);
}

void Art_case_data_init(Art_case_data* case_data)
{
    Zinc_string_init(&case_data->description);
    case_data->snapshot = false;
    case_data->has_error = false;
    case_data->ct = NULL;
    case_data->value = NULL;
    case_data->return_address = NULL;
    Zinc_input_bounds_init(&case_data->source_bounds);
    Zinc_input_bounds_init(&case_data->llvm_bounds);
    Zinc_string_init(&case_data->llvm);
    Zinc_spec_error_list_init(&case_data->spec_errors);
}

void Art_case_data_create(Art_case_data** case_data)
{
    Zinc_malloc_safe((void**)case_data, sizeof(Art_case_data));
    Art_case_data_init(*case_data);
}

void Art_case_data_destroy(Art_case_data* case_data)
{
    Zinc_string_destroy(&case_data->description);
    Cent_comp_table_destroy(case_data->ct);
    free(case_data->ct);
    Zinc_string_destroy(&case_data->llvm);
    Zinc_spec_error_list_destroy(&case_data->spec_errors);
    free(case_data->return_address);
}

void Art_suite_data_init(Art_suite_data* suite_data)
{
    Zinc_string_init(&suite_data->path);
    Zinc_string_init(&suite_data->description);
    Zinc_string_init(&suite_data->name);
    Zinc_error_list_init(&suite_data->errors);
}

void Art_suite_data_create(Art_suite_data** suite_data)
{
    Zinc_malloc_safe((void**)suite_data, sizeof(Art_suite_data));
    Art_suite_data_init(*suite_data);
}

void Art_suite_data_destroy(Art_suite_data* suite_data)
{
    Zinc_string_destroy(&suite_data->path);
    Zinc_string_destroy(&suite_data->description);
    Zinc_string_destroy(&suite_data->name);
    Zinc_error_list_destroy(&suite_data->errors);
}

void Art_top_data_init(Art_top_data* top_data)
{
    Zinc_string_init(&top_data->dir_path);
    top_data->has_solo = false;
    top_data->type_info = NULL;
    top_data->regex_re = Cob_compile_str("^/(.*)/\n?$");
}

void Art_top_data_create(Art_top_data** top_data)
{
    Zinc_malloc_safe((void**)top_data, sizeof(Art_top_data));
    Art_top_data_init(*top_data);
}

void Art_top_data_destroy(Art_top_data* top_data)
{
    Zinc_string_destroy(&top_data->dir_path);
    Cent_comp_table_destroy(top_data->type_info);
    free(top_data->type_info);
    Cob_re_destroy(&top_data->regex_re);
}