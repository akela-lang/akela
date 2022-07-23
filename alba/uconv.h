#ifndef _UCONV_H
#define _UCONV_H

#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>

#define IS_ONE_BYTE(c) (((c) & 0x80) == 0x00)
#define IS_TWO_BYTES(c) (((c) & 0xe0) == 0xc0)
#define IS_THREE_BYTES(c) (((c) & 0xf0) == 0xe0)
#define IS_FOUR_BYTES(c) (((c) & 0xf8) == 0xf0)

#define NUM_BYTES(c) (IS_ONE_BYTE(c) ? 1 : IS_TWO_BYTES(c) ? 2 : IS_THREE_BYTES(c) ? 3 : IS_FOUR_BYTES(c) ? 4 : 0)

#define IS_EXTRA_BYTE(c) (((c) & 0xc0) == 0x80)

/* dynamic-output dest */
/* resource-input conv */
/* resource-use conv */
enum result char2uchar(UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len);

/* dynamic-output dest */
/* resource-input conv */
/* resource-use conv */
enum result uchar2char(UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len);

/* dynamic-output-none */
/* resource-output conv */
enum result conv_open(UConverter** conv);

/* dynamic-output-none */
/* resource-destroy conv */
void conv_close(UConverter* conv);

#endif
