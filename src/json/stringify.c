#include "dom.h"
#include <assert.h>
#include "zinc/utf8.h"
#include "lex_tools.h"
#include "zinc/unicode.h"
#include "stringify.h"

void Json_stringify_string(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf);
void Json_stringify_number(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf);
void Json_stringify_array(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf);
void Json_stringify_object(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf);
void Json_stringify_property(struct Zinc_string* name, Json_dom* value);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Json_stringify(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf)
{
    if (dom->type == Json_dom_type_null) {
        Zinc_string_add_str(bf, "null");
        return;
    }

    if (dom->type == Json_dom_type_boolean) {
        if (dom->value.boolean) {
            Zinc_string_add_str(bf, "true");
        } else {
            Zinc_string_add_str(bf, "false");
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

void Json_escape_buffer(struct Zinc_error_list* el, struct Zinc_string* src, struct Zinc_string *bf)
{
    size_t i = 0;
    while (i < src->size) {
        char* c = src->buf + i;
        int num = ZINC_NUM_BYTES(c[0]);
        int byte_count = num;
        while (i + byte_count - 1 >= src->size) {
            byte_count--;
        }
        uint32_t cp;
        Zinc_utf8_to_utf32(c, &cp);
        char escape_char;
        bool must_escape = Json_must_escape(c, num, &escape_char);
        if (!must_escape && (cp < 0x20 || cp > 0x10ffff)) {
            struct Zinc_location loc;
            Zinc_location_init(&loc);
            Zinc_error_list_set(el, &loc, "invalid string character");
            i += num;
            continue;
        }
        if (num == 1) {
            if (must_escape) {
                Zinc_string_add_char(bf, '\\');
                Zinc_string_add_char(bf, escape_char);
            } else {
                Zinc_string_add_char(bf, c[0]);
            }
        } else {
            struct Zinc_string num_bf;
            Zinc_string_init(&num_bf);
            int len = Zinc_num32_to_hex_str(cp, &num_bf);

            struct Zinc_string seq_bf;
            Zinc_string_init(&seq_bf);
            Zinc_string_add_str(&seq_bf, "\\u");
            for (int j = 0; j < 4 - len; j++) {
                Zinc_string_add_char(&seq_bf, '0');
            }
            Zinc_string_copy(&num_bf, &seq_bf);
            Zinc_string_copy(&seq_bf, bf);

            Zinc_string_destroy(&num_bf);
            Zinc_string_destroy(&seq_bf);
        }
        i += num;
    }
}

void Json_stringify_string(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf)
{
    Zinc_string_add_char(bf, '"');
    Json_escape_buffer(el, &dom->value.string, bf);
    Zinc_string_add_char(bf, '"');
}

void Json_stringify_number(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf)
{
    if (dom->number_type == Json_number_type_integer) {
        Zinc_string_add_format(bf, "%lld", dom->value.integer);
    } else if (dom->number_type == Json_number_type_fp) {
        Zinc_string_add_format(bf, "%lf", dom->value.fp);
    } else {
        struct Zinc_location loc;
        Zinc_location_init(&loc);
        Zinc_error_list_set(el, &loc, "invalid number type");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Json_stringify_array(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf)
{
    Zinc_string_add_char(bf, '[');
    Json_dom* p = dom->head;
    size_t i = 0;
    while (p) {
        if (i != 0) {
            Zinc_string_add_char(bf, ',');
        }
        Json_stringify(el, p, bf);
        p = p->next;
        i++;
    }
    Zinc_string_add_char(bf, ']');
}

size_t Json_stringify_object_index = 0;
struct Zinc_error_list* Json_stringify_object_el = NULL;
struct Zinc_string* Json_stringify_object_bf = NULL;
void Json_stringify_object(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf)
{
    Zinc_string_add_char(bf, '{');
    Json_stringify_object_el = el;
    Json_stringify_object_bf = bf;
    Json_stringify_object_index = 0;
    Zinc_hash_map_string_map_name(&dom->value.object, (Zinc_hash_map_string_func_name)Json_stringify_property);
    Zinc_string_add_char(bf, '}');
}

void Json_stringify_property(struct Zinc_string* name, Json_dom* value)
{
    if (Json_stringify_object_index >= 1) {
        Zinc_string_add_char(Json_stringify_object_bf, ',');
    }

    Zinc_string_add_char(Json_stringify_object_bf, '"');
    Json_escape_buffer(Json_stringify_object_el, name, Json_stringify_object_bf);
    Zinc_string_add_char(Json_stringify_object_bf, '"');

    Zinc_string_add_char(Json_stringify_object_bf, ':');

    Json_stringify(Json_stringify_object_el, value, Json_stringify_object_bf);

    Json_stringify_object_index++;
}