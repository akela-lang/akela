#ifndef COVERAGE_PARSE_H
#define COVERAGE_PARSE_H

#include "cobble/compile.h"
#include "cobble/compile_data.h"

Cob_re Cover_gcov_line_re();
Cob_re Cover_gcov_ext_re();
void Cover_re_cleanup(Cob_re* re);
Cob_re Cover_test_dir_re();

#endif