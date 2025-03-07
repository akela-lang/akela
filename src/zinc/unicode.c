#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "zstring.h"
#include <stdbool.h>
#include <stdint.h>

bool Zinc_is_hex_digit(char c[4], int num)
{
    if (num != 1)
        return false;
    if (c[0] >= 'A' && c[0] <= 'F') return true;
    if (c[0] >= 'a' && c[0] <= 'f') return true;
    if (c[0] >= '0' && c[0] <= '9') return true;
    return false;
}

int Zinc_code_to_utf8(unsigned char *const buffer, const unsigned int code)
{
    if (code <= 0x7F) {
        buffer[0] = code;
        return 1;
    }
    if (code <= 0x7FF) {
        buffer[0] = 0xC0 | (code >> 6);            /* 110xxxxx */
        buffer[1] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 2;
    }
    if (code <= 0xFFFF) {
        buffer[0] = 0xE0 | (code >> 12);           /* 1110xxxx */
        buffer[1] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[2] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 3;
    }
    if (code <= 0x10FFFF) {
        buffer[0] = 0xF0 | (code >> 18);           /* 11110xxx */
        buffer[1] = 0x80 | ((code >> 12) & 0x3F);  /* 10xxxxxx */
        buffer[2] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[3] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 4;
    }
    return 0;
}

int Zinc_utf8_to_utf32(const char *utf8, uint32_t *cp)
{
    uint32_t codepoint = 0;
    unsigned char c = (unsigned char)utf8[0];
    int bytes = 0;

    if (c < 0x80) {  // 1-byte sequence
        codepoint = c;
        bytes = 1;
    } else if ((c & 0xE0) == 0xC0) {  // 2-byte sequence
        codepoint = (utf8[0] & 0x1F) << 6 |
                    (utf8[1] & 0x3F);
        bytes = 2;
    } else if ((c & 0xF0) == 0xE0) {  // 3-byte sequence
        codepoint = (utf8[0] & 0x0F) << 12 |
                    (utf8[1] & 0x3F) << 6 |
                    (utf8[2] & 0x3F);
        bytes = 3;
    } else if ((c & 0xF8) == 0xF0) {  // 4-byte sequence
        codepoint = (utf8[0] & 0x07) << 18 |
                    (utf8[1] & 0x3F) << 12 |
                    (utf8[2] & 0x3F) << 6 |
                    (utf8[3] & 0x3F);
        bytes = 4;
    }

    *cp = codepoint;
    return bytes;
}

unsigned int Zinc_char_to_hex(char c)
{
    int x = toupper(c);
    if (x == '0') return 0;
    if (x == '1') return 1;
    if (x == '2') return 2;
    if (x == '3') return 3;
    if (x == '4') return 4;
    if (x == '5') return 5;
    if (x == '6') return 6;
    if (x == '7') return 7;
    if (x == '8') return 8;
    if (x == '9') return 9;
    if (x == 'A') return 10;
    if (x == 'B') return 11;
    if (x == 'C') return 12;
    if (x == 'D') return 13;
    if (x == 'E') return 14;
    if (x == 'F') return 15;
    return 0;
}

unsigned int Zinc_char4_to_hex(char* src, int num)
{
    unsigned int code = 0;
    if (num > 0) {
        code = Zinc_char_to_hex(src[0]);
    }
    for (int i = 1; i < num; i++) {
        code = (code << 4) + Zinc_char_to_hex(src[i]);
    }
    return code;
}

char Zinc_num4_to_hex_char(uint8_t cp)
{
    if (cp == 0) return '0';
    if (cp == 1) return '1';
    if (cp == 2) return '2';
    if (cp == 3) return '3';
    if (cp == 4) return '4';
    if (cp == 5) return '5';
    if (cp == 6) return '6';
    if (cp == 7) return '7';
    if (cp == 8) return '8';
    if (cp == 9) return '9';
    if (cp == 10) return 'a';
    if (cp == 11) return 'b';
    if (cp == 12) return 'c';
    if (cp == 13) return 'd';
    if (cp == 14) return 'e';
    if (cp == 15) return 'f';
    return '0';
}

int Zinc_num32_to_hex_str(uint32_t cp, Zinc_string* bf)
{
    Zinc_string temp;
    Zinc_string_init(&temp);
    int len = 0;

    while (cp) {
        Zinc_string_add_char(&temp, Zinc_num4_to_hex_char(cp & 0xf));
        cp = cp >> 4;
        len++;
    }

    if (temp.size > 0) {
        size_t i = temp.size - 1;
        while (true) {
            Zinc_string_add_char(bf, temp.buf[i]);
            if (i == 0) break;
            i--;
        }
    }

    Zinc_string_destroy(&temp);

    return len;
}