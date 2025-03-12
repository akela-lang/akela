#include "data.h"
#include "parse_tools.h"
#include "cobble/match.h"

void Apt_get_line(Apt_data* data, FILE* fp, Zinc_string* line, Apt_line_kind* kind)
{
    *kind = Apt_line_kind_regular;
    Zinc_string_clear(line);

    int c;
    while ((c = fgetc(fp)) != EOF) {
        Zinc_string_add_char(line, (char)c);
        if (c == '\n') {
            break;
        }
    }

    if (c == EOF) {
        if (line->size == 0) {
            *kind = Apt_line_kind_eof;
            return;
        }
    }

    Zinc_string_slice slice = {.p = line->buf, .size = line->size};
    Cob_result ts_mr = Cob_match(&data->test_suite_sep, slice);
    Cob_result tc_mr = Cob_match(&data->test_case_sep, slice);
    Cob_result sec_mr = Cob_match(&data->section_sep, slice);
    if (ts_mr.matched) {
        *kind = Apt_line_kind_test_suite_end;
    } else if (tc_mr.matched) {
        *kind = Apt_line_kind_test_case_end;
    } else if (sec_mr.matched) {
        *kind = Apt_line_kind_section_end;
    }
    Cob_result_destroy(&ts_mr);
    Cob_result_destroy(&tc_mr);
    Cob_result_destroy(&sec_mr);
}
