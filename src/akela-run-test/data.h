#ifndef AKELA_RUN_TEST_DATA_H
#define AKELA_RUN_TEST_DATA_H

#include "cobble/compile.h"

typedef struct Run_data {
    Cob_re ext_re;
    Cob_re llvm_re;
    Cob_re result_re;
} Run_data;

void Run_data_init(Run_data* data);
void Run_data_create(Run_data** data);
void Run_data_destroy(Run_data* data);

#endif