#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "result.h"
#include "scan.h"
#include "ustring.h"

enum result_enum scan(struct string* line)
{
    enum result_enum r;
    size_t pos = 0;
    struct string s;
    string_init(&s);

    UErrorCode err;
    UConverter* conv = ucnv_open("utf8", &err);
    if (U_FAILURE(err)) {
        return set_error("utf error");
    }

    UChar* dest;
    size_t dest_len;
    r = char2uchar(conv, line->buf, line->size, &dest, line->size, &dest_len);
    if (r == error_result) {
        return r;
    }

    size_t len;

    UChar* space;
    r = char2uchar(conv, " ", 1, &space, 1, &len);
    if (r == error_result) {
        return r;
    }

    UChar* plus;
    r = char2uchar(conv, "+", 1, &plus, 1, &len);
    if (r == error_result) {
        return r;
    }

    while (pos < dest_len) {
        UChar32 c2;
        U16_NEXT(dest, pos, line->size, c2);

        if (u_isalpha(c2)) {
            printf("found alpha\n");
        } else if (u_isdigit(c2)) {
            printf("found digit\n");
        } else if (u_strFindFirst(dest + pos - 1, 1, plus, 1) != NULL) {
            printf("found plus\n");
        } else if (u_strFindFirst(dest + pos - 1, 1, space, 1) != NULL) {
            printf("found space\n");
        } else {
            char* a;
            r = uchar2char(conv, dest + pos - 1, 1, &a, 1, &len);
            if (r == error_result) {
                return r;
            }
            printf("unrecogized: %s\n", a);
            free(a);
        }
    }

    string_reset(&s);
    ucnv_close(conv);
    free(dest);
    free(space);

    return ok_result;
}
