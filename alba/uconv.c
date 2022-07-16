#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "zinc/memory.h"

/* static-output */
enum result check_num_bytes(unsigned char c, int* count)
{
    /* 1 byte: 0xxx xxxx */
    if ((c & 0x80) == 0x00) {
        *count = 1;
        return result_ok;
    }

    /* 2 byte: 110x xxxx */
    if ((c & 0xe0) == 0xc0) {
        *count = 2;
        return result_ok;
    }

    /* 3 byte: 1110 xxxx */
    if ((c & 0xf0) == 0xe0) {
        *count = 3;
        return result_ok;
    }

    /* 4 byte: 1111 0xxx */
    if ((c & 0xf8) == 0xf0) {
        *count = 3;
        return result_ok;
    }

    *count = 0;
    return set_error("Not utf8: could not detect number bytes in character");
}

/* static-output */
enum result check_extra_byte(char c)
{
    /* 10xx xxxx */
    if ((c & 0xc0) == 0x80) {
        return result_ok;
    }

    return set_error("Not utf8: extra byte in character not encoded as utf8");
}

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

/* static-output */
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

/* static-output */
/* resource-destroy conv */
void conv_close(UConverter* conv)
{
    if (conv) {
        ucnv_close(conv);
    }
}
