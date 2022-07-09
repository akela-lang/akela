#ifndef _UCONV_H
#define _UCONV_H

#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>

#define IS_ONE_BYTE(c) (((c) & 0x80) == 0x00)
#define IS_TWO_BYTES(c) (((c) & 0xe0) == 0xc0)
#define IS_THREE_BYTES(c) (((c) & 0xf0) == 0xe0)
#define IS_FOUR_BYTES(c) (((c) & 0xf8) == 0xf0)

#define NUM_BYTES(c) IS_ONE_BYTE(c) ? 1 : IS_TWO_BYTES(c) ? 2 : IS_THREE_BYTES(c) ? 3 : IS_FOUR_BYTES(c) ? 4 : 0

#define IS_EXTRA_BYTE(c) (((c) & 0xc0) == 0x80)

enum result check_num_bytes(unsigned char c, int* count);
enum result check_extra_byte(char c);
enum result char2uchar(struct allocator* al, UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len);
enum result uchar2char(struct allocator* al, UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len);
enum result conv_open(UConverter** conv);
void conv_close(UConverter* conv);

#endif
