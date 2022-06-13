#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "result.h"

enum result char2uchar(struct allocator* al, UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len)
{
    enum result r = allocator_malloc(al, dest, sizeof(UChar) * dest_size);
    if (r == result_error) {
        return r;
    }
    UErrorCode err;
    *len = ucnv_toUChars(conv, *dest, dest_size, src, src_size, &err);
    if (U_FAILURE(err)) {
        return set_error("utf error");
    }
    return result_ok;
}

enum result uchar2char(struct allocator* al, UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len)
{
    enum result r = allocator_malloc(al, dest, dest_size + 1);
    if (r == result_error) {
        return r;
    }
    UErrorCode err;
    *len = ucnv_fromUChars(conv, *dest, dest_size, src, src_size, &err);
    if (U_FAILURE(err)) {
        return set_error("utf error");
    }
    (*dest)[*len] = '\0';
    return result_ok;
}

enum result conv_open(UConverter** conv)
{
    UErrorCode err;
    *conv = ucnv_open("utf8", &err);
    if (U_FAILURE(err)) {
        return set_error("utf conversion: %d", err);
    }
    return result_ok;
}

void conv_close(UConverter* conv)
{
    if (conv) {
        ucnv_close(conv);
    }
}
