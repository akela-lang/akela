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
typedef enum Zinc_result (*Zinc_input_unicode_next_interface)(
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
typedef void (*Zinc_input_unicode_get_all_interface)(void*, Vector**);

/**
 * Get the current location.
 */
typedef Zinc_location(*Zinc_input_unicode_get_location_interface)(void*);

typedef struct {
    u_int8_t loc_offset;
    u_int8_t next_offset;
    u_int8_t repeat_offset;
    u_int8_t seek_offset;
    u_int8_t get_all_offset;
    u_int8_t get_location_offset;
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
    Vector** text);
Zinc_location Zinc_input_unicode_get_location(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable);

#endif