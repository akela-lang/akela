#ifndef LAVA_AST_H
#define LAVA_AST_H
#include <zinc/zstring.h>

typedef enum Lava_item_kind Lava_item_kind;
enum Lava_item_kind {
    Lava_item_kind_none,
    Lava_item_kind_text,
    Lava_item_kind_blockquote,
};

typedef struct Lava_item Lava_item;
struct Lava_item {
    Lava_item_kind kind;
    Zinc_string text;
    Lava_item* next;
    Lava_item* prev;
};

typedef struct Lava_item_list Lava_item_list;
struct Lava_item_list {
    Lava_item* head;
    Lava_item* tail;
};

typedef struct Lava_section Lava_section;
struct Lava_section {
    Zinc_string title;
    int level;
    Lava_item_list items;
};

#endif