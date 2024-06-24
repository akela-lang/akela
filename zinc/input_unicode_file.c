#include "input_unicode_file.h"
#include "memory.h"
#include <stddef.h>
#include "vector.h"
#include "utf8.h"
#include <string.h>

InputUnicodeVTable InputUnicodeFileVTable = {
        .loc_offset = offsetof(InputUnicodeFile, loc),
        .next_offset = offsetof(InputUnicodeFile, Next),
        .repeat_offset = offsetof(InputUnicodeFile, Repeat),
        .seek_offset = offsetof(InputUnicodeFile, Seek),
        .get_all_offset = offsetof(InputUnicodeFile, GetAll),
        .get_location_offset = offsetof(InputUnicodeFile, GetLocation),
};

void InputUnicodeFileInit(InputUnicodeFile* input, FILE* fp)
{
    location_init(&input->loc);
    input->repeat_char = false;
    memset(input->prev_c, 0, 4);
    input->prev_num = 0;
    location_init(&input->prev_loc);
    input->prev_done = false;
    input->fp = fp;
    input->Next = (InputUnicodeNextInterface) InputUnicodeFileNext;
    input->Repeat = (InputUnicodeRepeatInterface) InputUnicodeFileRepeat;
    input->Seek = (InputUnicodeSeekInterface) InputUnicodeFileSeek;
    input->GetAll = (InputUnicodeGetAllInterface) InputUnicodeFileGetAll;
    input->GetLocation = (InputUnicodeGetLocationInterface) InputUnicodeFileGetLocation;
    input->input_vtable = &InputUnicodeFileVTable;
}

void InputUnicodeFileCreate(InputUnicodeFile** input, FILE* fp)
{
    malloc_safe((void**)input, sizeof(InputUnicodeFile));
    InputUnicodeFileInit(*input, fp);
}

void InputUnicodeFileClear(InputUnicodeFile* input)
{
    location_init(&input->loc);
    location_init(&input->prev_loc);
    input->loc.line = 1;
    input->loc.col = 1;
}

/**
 * Get the next character.
 * @param lex_data lexer data
 * @param c the next char
 * @return done
 */
enum result InputUnicodeFileNext(
        InputUnicodeFile* input,
        char c[4],
        int* num,
        struct location* loc,
        bool* done)
{
    enum result r = result_ok;
    *done = false;
    if (input->loc.start_pos == 0) {
        InputUnicodeFileClear(input);
    }

    if (input->repeat_char) {
        input->repeat_char = false;
        memcpy(c, input->prev_c, 4);
        *num = input->prev_num;
        *loc = input->prev_loc;
        *done = input->prev_done;
        input->loc = input->prev_loc;
    } else {
        int x = getc(input->fp);
        *loc = input->loc;
        if (x == EOF) {
            *num = 0;
            *done = true;
        }
        c[0] = (char)x;
        *num = NUM_BYTES(c[0]);
        for (int i = 1; i < *num; i++) {
            x = getc(input->fp);
            if (x == EOF) {
                r = set_error("EOF before last utf8 trailing byte");
                *num = i;
                *done = true;
                break;
            }
            c[i] = (char)x;
            if (!IS_EXTRA_BYTE(c[i])) {
                r = set_error("utf8 trailing byte invalid");
            }
        }
        memcpy(input->prev_c, c, *num);
        input->prev_num = *num;
        input->prev_loc = input->loc;
        input->prev_done = *done;
    }

    input->loc.start_pos += *num;
    if (*num == 1 && c[0] == '\n') {
        input->loc.line++;
        input->loc.col = 1;
    } else {
        input->loc.col++;
    }
    return r;
}

/**
 * Repeat the previous character.
 * @param input lexer data
 */
void InputUnicodeFileRepeat(InputUnicodeFile* input)
{
    input->repeat_char = true;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param input the input
 * @param pos position to go to
 */
void InputUnicodeFileSeek(InputUnicodeFile* input, struct location* loc)
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
void InputUnicodeFileGetAll(InputUnicodeFile* input, Vector** text)
{
    InputUnicodeFileClear(input);
    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    fseek(input->fp, 0, SEEK_SET);
    int x;
    int num;
    char c[4];
    do {
        x = getc(input->fp);
        if (x == EOF) break;
        c[0] = (char)x;
        num = NUM_BYTES(c[0]);
        for (int i = 1; i < num; i++) {
            x = getc(input->fp);
            c[i] = (char)x;
        }
        VectorAdd(v, c, num);
    } while (true);
    VectorAddNull(v);
    *text = v;
}

struct location InputUnicodeFileGetLocation(InputUnicodeFile* input)
{
    return input->loc;
}