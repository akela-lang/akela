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
#include "unit_input_unicode_string.h"
#include "unit_list.h"
#include "unit_memory.h"
#include "unit_os_unix.h"
#include "unit_piece.h"
#include "unit_priority_queue.h"
#include "unit_result.h"
#include "unit_spec_error.h"
#include "unit_test.h"
#include "unit_unicode.h"
#include "unit_utf8.h"

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
        Zinc_test_register(test, Zinc_unit_input_unicode_string);
        Zinc_test_register(test, Zinc_unit_list);
        Zinc_test_register(test, Zinc_unit_memory);
        Zinc_test_register(test, Zinc_unit_os_unix);
        Zinc_test_register(test, Zinc_unit_piece);
        Zinc_test_register(test, Zinc_unit_priority_queue);
        Zinc_test_register(test, Zinc_unit_result);
        Zinc_test_register(test, Zinc_unit_spec_error);
        Zinc_test_register(test, Zinc_unit_test);
        Zinc_test_register(test, Zinc_unit_unicode);
        Zinc_test_register(test, Zinc_unit_utf8);

        return;
    }

    Zinc_test_perform(test);
}