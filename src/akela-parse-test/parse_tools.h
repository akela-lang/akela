#ifndef AKELA_PARSE_TEST_PARSE_TOOLS_H
#define AKELA_PARSE_TEST_PARSE_TOOLS_H

#include "data.h"

typedef enum Apt_line_kind {
    Apt_line_kind_none,
    Apt_line_kind_regular,
    Apt_line_kind_test_suite_end,
    Apt_line_kind_test_case_end,
    Apt_line_kind_section_end,
    Apt_line_kind_eof,
} Apt_line_kind;

void Apt_get_line(Apt_data* data, FILE* fp, Zinc_string* line, Apt_line_kind* kind);

#endif