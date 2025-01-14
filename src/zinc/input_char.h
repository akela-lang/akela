#ifndef ZINC_INPUT_CHAR_H
#define ZINC_INPUT_CHAR_H

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
typedef bool (*InputCharNextInterface)(void*, char*, struct Zinc_location*);

/**
 * Repeat the last character
 * @param data the interface data
 */
typedef void (*InputCharRepeatInterface)(void*);

/**
 * Seek to position in input.
 * @param data the interface data
 * @param pos the position to go to
 */
typedef void (*InputCharSeekInterface)(void*, struct Zinc_location*);

/**
 * Get the entire input.
 * @param data the data
 * @param text the input text
 */
typedef void (*InputCharGetAllInterface)(void*, Vector**);

/**
 * Get the current location.
 */
typedef struct Zinc_location(*InputCharGetLocationInterface)(void*);

typedef struct {
    u_int8_t loc_offset;
    u_int8_t next_offset;
    u_int8_t repeat_offset;
    u_int8_t seek_offset;
    u_int8_t get_all_offset;
    u_int8_t get_location_offset;
} InputCharVTable;

struct Zinc_location* InputCharLocation(void* input_obj, InputCharVTable* input_vtable);
bool InputCharNext(void* input_obj, InputCharVTable* input_vtable, char* c, struct Zinc_location* loc);
void InputCharRepeat(void* input_obj, InputCharVTable* input_vtable);
void InputCharSeek(void* input_obj, InputCharVTable* input_vtable, struct Zinc_location*);
void InputCharGetAll(void* input_obj, InputCharVTable* input_vtable, Vector** text);
struct Zinc_location InputCharGetLocation(void* input_obj, InputCharVTable* input_vtable);

#endif