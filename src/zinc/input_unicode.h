#ifndef ZINC_INPUT_UNICODE_H
#define ZINC_INPUT_UNICODE_H

#include <stdbool.h>
#include "error.h"
#include "vector.h"
#include "input_bounds.h"

/**
 * Get the next character.
 * @param data interface data
 * @param c pointer to character
 * @param loc location of character
 * @return true if done, otherwise false
 */
typedef Zinc_result (*Zinc_input_unicode_next_interface)(
        void*,
        char[4],
        int*,
        Zinc_location*,
        bool*);

/**
 * Repeat the last character
 * @param data the interface data
 */
typedef void (*Zinc_input_unicode_repeat_interface)(void*);

/**
 * Seek to position in input.
 * @param data the interface data
 * @param pos the position to go to
 */
typedef void (*Zinc_input_unicode_seek_interface)(void*, Zinc_location*);

/**
 * Get the entire input.
 * @param data the data
 * @param text the input text
 */
typedef void (*Zinc_input_unicode_get_all_interface)(void*, Zinc_vector**);

/**
 * Get the current location.
 */
typedef Zinc_location (*Zinc_input_unicode_get_location_interface)(void*);

typedef void (*Zinc_input_unicode_destroy_interface)(void*);

typedef void (*Zinc_input_unicode_set_bounds_interface)(void*, Zinc_input_bounds*);

typedef struct {
    uint8_t loc_offset;
    uint8_t next_offset;
    uint8_t repeat_offset;
    uint8_t seek_offset;
    uint8_t get_all_offset;
    uint8_t get_location_offset;
    uint8_t destroy_offset;
    uint8_t set_bounds_offset;
} Zinc_input_unicode_vtable;

Zinc_location* Zinc_input_unicode_location(void* input_obj, Zinc_input_unicode_vtable* input_vtable);
enum Zinc_result Zinc_input_unicode_next(void* input_obj,
                      Zinc_input_unicode_vtable* input_vtable,
                      char c[4],
                      int* num,
                      Zinc_location* loc,
                      bool* done);
void Zinc_input_unicode_repeat(void* input_obj, Zinc_input_unicode_vtable* input_vtable);
void Zinc_input_unicode_seek(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_location* loc);
void Zinc_input_unicode_get_all(void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_vector** text);
Zinc_location Zinc_input_unicode_get_location(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable);
void Zinc_input_unicode_destroy(void* input_obj, Zinc_input_unicode_vtable* input_vtable);

void Zinc_input_unicode_set_bounds(void* input, Zinc_input_unicode_vtable* vtable, Zinc_input_bounds* bounds);

#endif