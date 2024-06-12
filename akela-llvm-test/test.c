#include "zinc/unit_test.h"
#include "test_akela_llvm_literal.h"
#include "test_akela_llvm_variable.h"
#include "test_akela_llvm_operator.h"
#include "test_akela_llvm_stmts.h"
#include "test_akela_llvm_function.h"

int main() {
    test_akela_llvm_literal();
    test_akela_llvm_variable();
    test_akela_llvm_operator();
    test_akela_llvm_stmts();
    test_akela_llvm_function();
    print_results();
    return 0;
}