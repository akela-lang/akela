#include "dom.h"
#include "parse_data.h"
#include "parse_tools.h"

Lava_result Lava_parse(Lava_parse_data* pd)
{

}

Lava_dom* Lava_parse_header(Lava_parse_data* pd)
{
    Lava_dom* n = NULL;
    Lava_dom_create(&n, LAVA_DOM_HEADER);

    Lava_token* hdr = NULL;
    Lava_match(pd, Lava_token_kind_header, "expected header", &hdr);

    if (hdr) {
        n->data.LAVA_DOM_HEADER.level = (int)hdr->text.size;
        Lava_token_destroy(hdr);
        free(hdr);
    }

    Lava_token* title = NULL;
    Lava_match(pd, Lava_token_kind_text, "expected header title", &title);

    if (title) {
        Zinc_string_add_string(&n->data.LAVA_DOM_HEADER.title, &title->text);
        Lava_token_destroy(title);
        free(title);
    }

    Lava_token* nl = NULL;
    Lava_match(pd, Lava_token_kind_newline, "expected newline", &nl);
    Lava_token_destroy(nl);
    free(nl);

    return n;
}