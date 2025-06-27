#include <stdbool.h>
#include "zinc/memory.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"
#include "akela-llvm/cg.h"
#include <string.h>
#include "zinc/input_unicode_string.h"
#include "zinc/string_list.h"
#include "zinc/expect.h"

void AkeLlvmUnit_chomp(Zinc_string* string)
{
    while (string->size > 0 && string->buf[string->size - 1] == '\n') {
        string->size--;
    }
}

bool AkeLlvmUnit_cg_setup(const char* text, Ake_code_gen_result* result)
{
    struct Ake_comp_unit* cu = NULL;
    Zinc_malloc_safe((void**)&cu, sizeof(struct Ake_comp_unit));

    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, text, strlen(text));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    bool valid = true;;

    Ake_comp_unit_init(cu);
    valid = Ake_comp_unit_compile(cu, input, input->vtable) && valid;
    result->cu = cu;
    if (!valid) {
        return valid;
    }

    Akela_llvm_cg* cg = NULL;
    Akela_llvm_cg_create(&cg, &cu->errors, &cu->extern_list);
    void* jd = Ake_code_gen_init(cg, &Akela_llvm_vtable);
    valid = Ake_code_gen_jit(cg, &Akela_llvm_vtable, jd, cu->root, result) && valid;
    Ake_code_gen_destroy(cg, &Akela_llvm_vtable, jd);
    Akela_llvm_cg_destroy(cg);

    if (!valid && result->module_text.size > 0) {
        struct Zinc_location loc;
        Zinc_location_init(&loc);
        Zinc_string_finish(&result->module_text);
        Zinc_error_list_set(&cu->errors, &loc, "Module:\n%s", result->module_text.buf);
    }

    Zinc_vector_destroy(vector);
    free(vector);
    free(input);

    AkeLlvmUnit_chomp(&result->value);

    return valid;
}