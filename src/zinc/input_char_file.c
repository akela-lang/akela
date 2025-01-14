#include "input_char_file.h"
#include "memory.h"
#include <stddef.h>
#include "vector.h"

InputCharVTable InputCharFileVTable = {
        .loc_offset = offsetof(InputCharFile, loc),
        .next_offset = offsetof(InputCharFile, Next),
        .repeat_offset = offsetof(InputCharFile, Repeat),
        .seek_offset = offsetof(InputCharFile, Seek),
        .get_all_offset = offsetof(InputCharFile, GetAll),
        .get_location_offset = offsetof(InputCharFile, GetLocation),
};

void InputCharFileInit(InputCharFile* input, FILE* fp)
{
    Zinc_location_init(&input->loc);
    Zinc_location_init(&input->prev_loc);
    input->repeat_char = false;
    input->prev_c = 0;
    input->fp = fp;
    input->Next = (InputCharNextInterface) InputCharFileNext;
    input->Repeat = (InputCharRepeatInterface) InputCharFileRepeat;
    input->Seek = (InputCharSeekInterface) InputCharFileSeek;
    input->GetAll = (InputCharGetAllInterface) InputCharFileGetAll;
    input->GetLocation = (InputCharGetLocationInterface) InputCharFileGetLocation;
    input->input_vtable = &InputCharFileVTable;
}

void InputCharFileCreate(InputCharFile** input, FILE* fp)
{
    malloc_safe((void**)input, sizeof(InputCharFile));
    InputCharFileInit(*input, fp);
}

void InputCharFileClear(InputCharFile* input)
{
    Zinc_location_init(&input->loc);
    Zinc_location_init(&input->prev_loc);
    input->loc.line = 1;
    input->loc.col = 1;
    input->loc.end_pos = 1;
}

/**
 * Get the next character.
 * @param lex_data lexer data
 * @param c the next char
 * @return done
 */
bool InputCharFileNext(InputCharFile* input, char* c, struct Zinc_location* loc)
{
    if (input->loc.start_pos == 0) {
        InputCharFileClear(input);
    }

    if (input->repeat_char) {
        input->loc = input->prev_loc;
        input->repeat_char = false;
        *c = input->prev_c;
        *loc = input->loc;
    } else {
        int x = getc(input->fp);
        *loc = input->loc;
        if (x == EOF) {
            return true;
        }
        *c = (char)x;
        input->prev_c = *c;
    }

    input->prev_loc = input->loc;
    input->loc.start_pos++;
    input->loc.end_pos = input->loc.start_pos + 1;
    if (*c == '\n') {
        input->loc.line++;
        input->loc.col = 1;
    } else {
        input->loc.col++;
    }
    return false;
}

/**
 * Repeat the previous character.
 * @param input lexer data
 */
void InputCharFileRepeat(InputCharFile* input)
{
    input->repeat_char = true;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param input the input
 * @param pos position to go to
 */
void InputCharFileSeek(InputCharFile* input, struct Zinc_location* loc)
{
    input->loc = *loc;
    input->prev_loc = *loc;
    fseek(input->fp, (long)loc->start_pos, SEEK_SET);
}

/**
 * Get entire input stream.
 * @param input the input
 * @param v the output vector
 */
void InputCharFileGetAll(InputCharFile* input, Vector** text)
{
    InputCharFileClear(input);
    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    fseek(input->fp, 0, SEEK_SET);
    do {
        int x = getc(input->fp);
        if (x == EOF) break;
        char c = (char)x;
        VectorAdd(v, &c, 1);
    } while (true);
    VectorAddNull(v);
    *text = v;
}

struct Zinc_location InputCharFileGetLocation(InputCharFile* input)
{
    return input->loc;
}