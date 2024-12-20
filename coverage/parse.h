#ifndef COVERAGE_PARSE_H
#define COVERAGE_PARSE_H

#include "cobble/compile.h"
#include "cobble/compile_data.h"

Cob_re Cov_gov_line_re();
void Cov_re_cleanup(Cob_re* cr);

#endif