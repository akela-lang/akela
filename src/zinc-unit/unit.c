#include "unit_string.h"
#include "unit_string_list.h"
#include "unit_string_slice.h"
#include "unit_datetime.h"
#include "unit_dbl_vector.h"
#include "unit_error.h"
#include "unit_hash_map_size_t.h"
#include "unit_hash_map_string.h"
#include "unit_input_char_file.h"
#include "unit_input_char_string.h"
#include "unit_input_unicode_file.h"

void Zinc_unit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_string);
        Zinc_test_register(test, Zinc_unit_string_list);
        Zinc_test_register(test, Zinc_unit_string_slice);
        Zinc_test_register(test, Zinc_unit_datetime);
        Zinc_test_register(test, Zinc_unit_dbl_vector);
        Zinc_test_register(test, Zinc_unit_error);
        Zinc_test_register(test, Zinc_unit_hash_map_size_t);
        Zinc_test_register(test, Zinc_unit_hash_map_string);
        Zinc_test_register(test, Zinc_unit_input_char_file);
        Zinc_test_register(test, Zinc_unit_input_char_string);
        Zinc_test_register(test, Zinc_unit_input_unicode_file);

        return;
    }

    Zinc_test_perform(test);
}