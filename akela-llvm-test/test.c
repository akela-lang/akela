#include "zinc/unit_test.h"
#include "test_akela_llvm.h"
#include "test_akela_llvm_literal.h"

int main() {
    test_akela_llvm_literal();
    test_code_gen();
    print_results();
    return 0;
}