#include "data.h"
#include "centipede/base.h"

Run_cent_data* Run_get_cent(Zinc_string* dir_path, Zinc_string* file_name, Zinc_string* config_string)
{
    Cent_module_file* mf = NULL;
    Cent_module_file_create(&mf, dir_path);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create(&ct, mf, mf->vtable);

    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(char));
    Zinc_vector_add(v, config_string->buf, config_string->size);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, v);

    Zinc_string_slice file_name_slice = {file_name->buf, file_name->size};
    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(
        &cu,
        input,
        input->input_vtable,
        file_name_slice,
        ct->base);
    Cent_comp_table_add_str(ct, "*config*", cu);
    ct->primary = cu;
    cu->ct = ct;
    cu->pd.cu = cu;
    cu->pd.ct = ct;

    bool valid = false;
    Cent_comp_unit_parse(cu);
    if (cu->errors.head) {
        Zinc_error* e = cu->errors.head;
        while (e) {
            Zinc_string_finish(&e->message);
            printf("(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
    } else {
        Cent_comp_unit_build(cu);
        if (cu->errors.head) {
            Zinc_error* e = cu->errors.head;
            while (e) {
                Zinc_string_finish(&e->message);
                printf("(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
                e = e->next;
            }
        } else {
            valid = true;
        }
    }

    Run_cent_data* config_data = NULL;
    Run_cent_data_create(&config_data, file_name);
    config_data->mf = mf;
    config_data->ct = ct;
    config_data->v = v;
    config_data->input = input;
    config_data->valid = valid;

    if (valid) {
        return config_data;
    }

    Run_cent_data_destroy(config_data);

    return NULL;
}