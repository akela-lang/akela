#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "zinc/memory.h"

/* dynamic-output dest */
/* resource-input conv */
/* resource-use conv */
enum result char2uchar(UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len)
{
    /* allocate dest */
    malloc_safe(dest, sizeof(UChar) * dest_size);

    /* use conv */
    UErrorCode err;
    *len = ucnv_toUChars(conv, *dest, dest_size, src, src_size, &err);
    if (U_FAILURE(err)) {
        return set_error("utf error");
    }

    return result_ok;
}

/* dynamic-output dest */
/* resource-input conv */
/* resource-use conv */
enum result uchar2char(UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len)
{
    /* allocate dest */
    malloc_safe(dest, dest_size + 1);
    UErrorCode err;
    /* use conv */
    *len = ucnv_fromUChars(conv, *dest, dest_size, src, src_size, &err);
    if (U_FAILURE(err)) {
        return set_error("utf error");
    }
    (*dest)[*len] = '\0';
    return result_ok;
}

/* dynamic-output-none */
/* resource-output conv */
enum result conv_open(UConverter** conv)
{
    UErrorCode err;
    *conv = ucnv_open("utf8", &err);
    if (U_FAILURE(err)) {
        return set_error("utf conversion: %d", err);
    }
    return result_ok;
}

/* dynamic-output-none */
/* resource-destroy conv */
void conv_close(UConverter* conv)
{
    if (conv) {
        ucnv_close(conv);
    }
}
