#include "dom.h"
#include <assert.h>

void Json_stringify(Json_dom* dom, struct buffer *bf)
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

    assert(false && "invalid dom type");
}