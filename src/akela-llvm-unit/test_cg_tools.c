#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "zinc/memory.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"
#include "akela-llvm/cg.h"
#include <string.h>
#include "zinc/input_unicode_string.h"
#include "zinc/string_list.h"

void test_chomp(Zinc_string* string)
{
    while (string->size > 0 && string->buf[string->size - 1] == '\n') {
        string->size--;
    }
}

bool cg_setup(const char* text, Ake_code_gen_result* result)
{
    struct Ake_comp_unit* cu = NULL;
    Zinc_malloc_safe((void**)&cu, sizeof(struct Ake_comp_unit));

    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, text, strlen(text));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    bool valid;

    Ake_comp_unit_init(cu);
    valid = Ake_comp_unit_compile(cu, input, input->vtable);
    Zinc_expect_true(valid, "valid");

    result->cu = cu;

    if (valid) {
        Akela_llvm_cg* cg = NULL;
        Akela_llvm_cg_create(&cg, &cu->errors, &cu->extern_list);
        valid = Ake_code_gen_jit(cg, &Akela_llvm_vtable, cu->root, result);
        Akela_llvm_cg_destroy(cg);
        Zinc_expect_true(valid, "valid");
    }

    if (!valid && result->module_text.size > 0) {
        struct Zinc_location loc;
        Zinc_location_init(&loc);
        Zinc_string_finish(&result->module_text);
        Zinc_error_list_set(&cu->errors, &loc, "Module:\n%s", result->module_text.buf);
    }
    Zinc_expect_no_errors(&cu->errors);

    Zinc_vector_destroy(vector);
    free(vector);
    free(input);

    test_chomp(&result->value);

    return valid;
}