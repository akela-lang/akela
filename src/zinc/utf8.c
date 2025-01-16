#include "utf8.h"
#include "result.h"

Zinc_result Zinc_check_num_bytes(unsigned char c, int* count)
{
    /* 1 byte: 0xxx xxxx */
    if ((c & 0x80) == 0x00) {
        *count = 1;
        return Zinc_result_ok;
    }

    /* 2 byte: 110x xxxx */
    if ((c & 0xe0) == 0xc0) {
        *count = 2;
        return Zinc_result_ok;
    }

    /* 3 byte: 1110 xxxx */
    if ((c & 0xf0) == 0xe0) {
        *count = 3;
        return Zinc_result_ok;
    }

    /* 4 byte: 1111 0xxx */
    if ((c & 0xf8) == 0xf0) {
        *count = 3;
        return Zinc_result_ok;
    }

    *count = 0;
    return Zinc_set_error("Not utf8: could not detect number bytes in character");
}

Zinc_result Zinc_check_extra_byte(char c)
{
    /* 10xx xxxx */
    if ((c & 0xc0) == 0x80) {
        return Zinc_result_ok;
    }

    return Zinc_set_error("Not utf8: extra byte in character not encoded as utf8");
}
