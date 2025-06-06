#ifndef ZINC_UTF8_H
#define ZINC_UTF8_H

#include "api.h"
#include "result.h"

#define ZINC_IS_ONE_BYTE(c) (((c) & 0x80) == 0x00)
#define ZINC_IS_TWO_BYTES(c) (((c) & 0xe0) == 0xc0)
#define ZINC_IS_THREE_BYTES(c) (((c) & 0xf0) == 0xe0)
#define ZINC_IS_FOUR_BYTES(c) (((c) & 0xf8) == 0xf0)
#define ZINC_NUM_BYTES(c) (ZINC_IS_ONE_BYTE(c) ? 1 : ZINC_IS_TWO_BYTES(c) ? 2 : ZINC_IS_THREE_BYTES(c) ? 3 : ZINC_IS_FOUR_BYTES(c) ? 4 : 0)
#define ZINC_IS_EXTRA_BYTE(c) (((c) & 0xc0) == 0x80)

ZINC_API Zinc_result Zinc_check_num_bytes(unsigned char c, int* count);
ZINC_API Zinc_result Zinc_check_extra_byte(char c);

#endif
