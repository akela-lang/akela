#ifndef _UTF8_H
#define _UTF8_H

#include "api.h"
#include "result.h"

#define IS_ONE_BYTE(c) (((c) & 0x80) == 0x00)
#define IS_TWO_BYTES(c) (((c) & 0xe0) == 0xc0)
#define IS_THREE_BYTES(c) (((c) & 0xf0) == 0xe0)
#define IS_FOUR_BYTES(c) (((c) & 0xf8) == 0xf0)

#define NUM_BYTES(c) (IS_ONE_BYTE(c) ? 1 : IS_TWO_BYTES(c) ? 2 : IS_THREE_BYTES(c) ? 3 : IS_FOUR_BYTES(c) ? 4 : 0)

#define IS_EXTRA_BYTE(c) (((c) & 0xc0) == 0x80)

ZINC_API enum result check_num_bytes(unsigned char c, int* count);
ZINC_API enum result check_extra_byte(char c);

#endif
