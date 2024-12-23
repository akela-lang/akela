#include "dom.h"
#include <assert.h>
#include "zinc/utf8.h"
#include "lex_tools.h"
#include "zinc/unicode.h"
#include "stringify.h"

void Json_stringify_string(struct error_list* el, Json_dom* dom, struct buffer *bf);
void Json_stringify_number(struct error_list* el, Json_dom* dom, struct buffer *bf);
void Json_stringify_array(struct error_list* el, Json_dom* dom, struct buffer *bf);
void Json_stringify_object(struct error_list* el, Json_dom* dom, struct buffer *bf);
void Json_stringify_property(struct buffer* name, Json_dom* value);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Json_stringify(struct error_list* el, Json_dom* dom, struct buffer *bf)
{
    if (dom->type == Json_dom_type_null) {
        buffer_add_str(bf, "null");
        return;
    }

    if (dom->type == Json_dom_type_boolean) {
        if (dom->value.boolean) {
            buffer_add_str(bf, "true");
        } else {
            buffer_add_str(bf, "false");
        }
        return;
    }

    if (dom->type == Json_dom_type_string) {
        Json_stringify_string(el, dom, bf);
        return;
    }

    if (dom->type == Json_dom_type_number) {
        Json_stringify_number(el, dom, bf);
        return;
    }

    if (dom->type == Json_dom_type_array) {
        Json_stringify_array(el, dom, bf);
        return;
    }

    if (dom->type == Json_dom_type_object) {
        Json_stringify_object(el, dom, bf);
        return;
    }

    assert(false && "invalid dom type");
}

bool Json_must_escape(char c[4], int num, char* escape_char)
{
    if (num == 1) {
        if (c[0] == '"') {
            *escape_char = '"';
            return true;
        }

        if (c[0] == '\\') {
            *escape_char = '\\';
            return true;
        }

        if (c[0] == '/') {
            *escape_char = '/';
            return true;
        }

        if (c[0] == '\b') {
            *escape_char = 'b';
            return true;
        }

        if (c[0] == '\f') {
            *escape_char = 'f';
            return true;
        }

        if (c[0] == '\n') {
            *escape_char = 'n';
            return true;
        }

        if (c[0] == '\r') {
            *escape_char = 'r';
            return true;
        }

        if (c[0] == '\t') {
            *escape_char = 't';
            return true;
        }
    }

    return false;
}

void Json_escape_buffer(struct error_list* el, struct buffer* src, struct buffer *bf)
{
    size_t i = 0;
    while (i < src->size) {
        char* c = src->buf + i;
        int num = NUM_BYTES(c[0]);
        int byte_count = num;
        while (i + byte_count - 1 >= src->size) {
            byte_count--;
        }
        UChar32 cp;
        Json_convert_char(c, byte_count, &cp);
        char escape_char;
        bool must_escape = Json_must_escape(c, num, &escape_char);
        if (!must_escape && (cp < 0x20 || cp > 0x10ffff)) {
            struct location loc;
            location_init(&loc);
            error_list_set(el, &loc, "invalid string character");
            i += num;
            continue;
        }
        if (num == 1) {
            if (must_escape) {
                buffer_add_char(bf, '\\');
                buffer_add_char(bf, escape_char);
            } else {
                buffer_add_char(bf, c[0]);
            }
        } else {
            struct buffer num_bf;
            buffer_init(&num_bf);
            int len = num32_to_hex_str(cp, &num_bf);

            struct buffer seq_bf;
            buffer_init(&seq_bf);
            buffer_copy_str(&seq_bf, "\\u");
            for (int j = 0; j < 4 - len; j++) {
                buffer_add_char(&seq_bf, '0');
            }
            buffer_copy(&num_bf, &seq_bf);
            buffer_copy(&seq_bf, bf);

            buffer_destroy(&num_bf);
            buffer_destroy(&seq_bf);
        }
        i += num;
    }
}

void Json_stringify_string(struct error_list* el, Json_dom* dom, struct buffer *bf)
{
    buffer_add_char(bf, '"');
    Json_escape_buffer(el, &dom->value.string, bf);
    buffer_add_char(bf, '"');
}

void Json_stringify_number(struct error_list* el, Json_dom* dom, struct buffer *bf)
{
    if (dom->number_type == Json_dom_number_type_integer) {
        buffer_add_format(bf, "%lld", dom->value.integer);
    } else if (dom->number_type == Json_dom_number_type_fp) {
        buffer_add_format(bf, "%lf", dom->value.fp);
    } else {
        struct location loc;
        location_init(&loc);
        error_list_set(el, &loc, "invalid number type");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Json_stringify_array(struct error_list* el, Json_dom* dom, struct buffer *bf)
{
    buffer_add_char(bf, '[');
    Json_dom* p = dom->head;
    size_t i = 0;
    while (p) {
        if (i != 0) {
            buffer_add_char(bf, ',');
        }
        Json_stringify(el, p, bf);
        p = p->next;
        i++;
    }
    buffer_add_char(bf, ']');
}

size_t Json_stringify_object_index = 0;
struct error_list* Json_stringify_object_el = NULL;
struct buffer* Json_stringify_object_bf = NULL;
void Json_stringify_object(struct error_list* el, Json_dom* dom, struct buffer *bf)
{
    buffer_add_char(bf, '{');
    Json_stringify_object_el = el;
    Json_stringify_object_bf = bf;
    Json_stringify_object_index = 0;
    hash_table_map_name(&dom->value.object, (hash_table_func_name)Json_stringify_property);
    buffer_add_char(bf, '}');
}

void Json_stringify_property(struct buffer* name, Json_dom* value)
{
    if (Json_stringify_object_index >= 1) {
        buffer_add_char(Json_stringify_object_bf, ',');
    }

    buffer_add_char(Json_stringify_object_bf, '"');
    Json_escape_buffer(Json_stringify_object_el, name, Json_stringify_object_bf);
    buffer_add_char(Json_stringify_object_bf, '"');

    buffer_add_char(Json_stringify_object_bf, ':');

    Json_stringify(Json_stringify_object_el, value, Json_stringify_object_bf);

    Json_stringify_object_index++;
}