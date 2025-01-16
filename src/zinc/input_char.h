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
typedef bool (*Zinc_input_char_next_interface)(void*, char*, Zinc_location*);

/**
 * Repeat the last character
 * @param data the interface data
 */
typedef void (*Zinc_input_char_repeat_interface)(void*);

/**
 * Seek to position in input.
 * @param data the interface data
 * @param pos the position to go to
 */
typedef void (*Zinc_input_char_seek_interface)(void*, Zinc_location*);

/**
 * Get the entire input.
 * @param data the data
 * @param text the input text
 */
typedef void (*Zinc_input_char_get_all_interface)(void*, Zinc_vector**);

/**
 * Get the current location.
 */
typedef Zinc_location(*Zinc_input_char_get_location_interface)(void*);

typedef struct {
    u_int8_t loc_offset;
    u_int8_t next_offset;
    u_int8_t repeat_offset;
    u_int8_t seek_offset;
    u_int8_t get_all_offset;
    u_int8_t get_location_offset;
} Zinc_input_char_vtable;

Zinc_location* Zinc_input_char_location(void* input_obj, Zinc_input_char_vtable* input_vtable);
bool Zinc_input_char_next(
    void* input_obj,
    Zinc_input_char_vtable*
    input_vtable,
    char* c,
    Zinc_location* loc);
void Zinc_input_char_repeat(void* input_obj, Zinc_input_char_vtable* input_vtable);
void Zinc_input_char_seek(void* input_obj, Zinc_input_char_vtable* input_vtable, Zinc_location*);
void Zinc_input_char_get_all(void* input_obj, Zinc_input_char_vtable* input_vtable, Zinc_vector** text);
Zinc_location Zinc_input_input_char_get_location(void* input_obj, Zinc_input_char_vtable* input_vtable);

#endif