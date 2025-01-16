#include "zinc/unit_test.h"
#include "test_akela_llvm_literal.h"
#include "test_akela_llvm_variable.h"
#include "test_akela_llvm_operator.h"
#include "test_akela_llvm_stmts.h"
#include "test_akela_llvm_function.h"
#include "test_akela_llvm_array.h"
#include "test_akela_llvm_struct.h"

int main() {
    test_akela_llvm_literal();
    test_akela_llvm_variable();
    test_akela_llvm_operator();
    test_akela_llvm_stmts();
    test_akela_llvm_array();
    test_akela_llvm_function();
    test_akela_llvm_struct();
    Zinc_print_results();
    return 0;
}