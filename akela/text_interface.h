#ifndef TEXT_INTERFACE_H
#define TEXT_INTERFACE_H

#include <stdlib.h>
#include <stdbool.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "akela_api.h"

struct range {
    const char* s;
    size_t start;
    size_t end;
};

struct get_text_data {
    struct range text;
    bool input_done;
    void* input_data;
};

typedef void (*get_text_interface)(struct get_text_data*);
AKELA_API void get_text_data_init(struct get_text_data* gtd, void* input_data);

struct char_info {
    char* utf8;
    UChar32* utf32;
};

struct get_char_data {
    struct char_info info;
    bool input_done;
    void* input_data;
};

typedef void (*get_char_interface)(struct get_char_data*);
AKELA_API void get_char_data_init(struct get_char_data* gcd, void* input_data);

#endif