#ifndef ZINC_INPUT_UNICODE_H
#define ZINC_INPUT_UNICODE_H

#include <stdbool.h>
#include "error.h"
#include "vector.h"

/**
 * Get the next character.
 * @param data interface data
 * @param c pointer to character
 * @param loc location of character
 * @return true if done, otherwise false
 */
typedef enum result (*InputUnicodeNextInterface)(
        void*,
        char[4],
        int*,
        struct location*,
        bool*);

/**
 * Repeat the last character
 * @param data the interface data
 */
typedef void (*InputUnicodeRepeatInterface)(void*);

/**
 * Seek to position in input.
 * @param data the interface data
 * @param pos the position to go to
 */
typedef void (*InputUnicodeSeekInterface)(void*, struct location*);

/**
 * Get the entire input.
 * @param data the data
 * @param text the input text
 */
typedef void (*InputUnicodeGetAllInterface)(void*, Vector**);

/**
 * Get the current location.
 */
typedef struct location(*InputUnicodeGetLocationInterface)(void*);

typedef struct {
    u_int8_t loc_offset;
    u_int8_t next_offset;
    u_int8_t repeat_offset;
    u_int8_t seek_offset;
    u_int8_t get_all_offset;
    u_int8_t get_location_offset;
} InputUnicodeVTable;

struct location* InputUnicodeLocation(void* input_obj, InputUnicodeVTable* input_vtable);
enum result InputUnicodeNext(void* input_obj,
                      InputUnicodeVTable* input_vtable,
                      char c[4],
                      int* num,
                      struct location* loc,
                      bool* done);
void InputUnicodeRepeat(void* input_obj, InputUnicodeVTable* input_vtable);
void InputUnicodeSeek(void* input_obj, InputUnicodeVTable* input_vtable, struct location* loc);
void InputUnicodeGetAll(void* input_obj, InputUnicodeVTable* input_vtable, Vector** text);
struct location InputUnicodeGetLocation(void* input_obj, InputUnicodeVTable* input_vtable);

#endif