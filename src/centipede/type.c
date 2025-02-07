#include "type.h"
#include "zinc/memory.h"

void Cent_type_param_init(Cent_type_param* tp)
{
    Zinc_string_init(&tp->name);
    tp->type = NULL;
    tp->when = Cent_when_run_time;
    tp->next = NULL;
    tp->prev = NULL;
}

void Cent_type_param_create(Cent_type_param** tp)
{
    Zinc_malloc_safe((void**)tp, sizeof(Cent_type_param));
    Cent_type_param_init(*tp);
}

void Cent_type_param_destroy(Cent_type_param* tp)
{
    Zinc_string_destroy(&tp->name);
}

void Cent_type_param_list_init(Cent_type_param_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cent_type_param_list_create(Cent_type_param_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Cent_type_param_list));
    Cent_type_param_list_init(*list);
}

void Cent_type_param_list_add(Cent_type_param_list* list, Cent_type_param* tp)
{
    if (list->head && list->tail) {
        list->tail->next = tp;
        tp->prev = list->tail;
        list->tail = tp;
    } else {
        list->head = tp;
        list->tail = tp;
    }
}

void Cent_type_param_list_destroy(Cent_type_param_list* list)
{
    Cent_type_param* p = list->head;
    while (p) {
        Cent_type_param* temp = p;
        p = p->next;
        Cent_type_param_destroy(temp);
        free(temp);
    }
}

void Cent_type_node_init(Cent_type_node* tn)
{
    tn->type = NULL;
    tn->next = NULL;
    tn->prev = NULL;
}

void Cent_type_node_create(Cent_type_node** tn)
{
    Zinc_malloc_safe((void**)tn, sizeof(Cent_type_node));
    Cent_type_node_init(*tn);
}

void Cent_type_node_destroy(Cent_type_node* tn)
{
}

void Cent_type_list_init(Cent_type_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cent_type_list_create(Cent_type_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Cent_type_list));
    Cent_type_list_init(*list);
}

void Cent_type_list_add(Cent_type_list* list, Cent_type_node* tn)
{
    if (list->head && list->tail) {
        list->tail->next = tn;
        tn->prev = list->tail;
        list->tail = tn;
    } else {
        list->head = tn;
        list->tail = tn;
    }
}

void Cent_type_list_destroy(Cent_type_list* list)
{
    Cent_type_node* tn = list->head;
    while (tn) {
        Cent_type_node* temp = tn;
        tn = tn->next;
        Cent_type_node_destroy(temp);
        free(temp);
    }
}

void Cent_type_field_init(Cent_type_field* tf)
{
    Zinc_string_init(&tf->name);
    tf->type = NULL;
    tf->next = NULL;
    tf->prev = NULL;
}

void Cent_type_field_create(Cent_type_field** tf)
{
    Zinc_malloc_safe((void**)tf, sizeof(Cent_type_field));
    Cent_type_field_init(*tf);
}

void Cent_type_field_destroy(Cent_type_field* tf)
{
    Zinc_string_destroy(&tf->name);
}

void Cent_type_field_list_init(Cent_type_field_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cent_type_field_list_create(Cent_type_field_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Cent_type_field));
    Cent_type_field_list_init(*list);
}

void Cent_type_field_list_add(Cent_type_field_list* list, Cent_type_field* tf)
{
    if (list->head && list->tail) {
        list->tail->next = tf;
        tf->prev = list->tail;
        list->tail = tf;
    } else {
        list->head = tf;
        list->tail = tf;
    }
}

void Cent_type_field_list_destroy(Cent_type_field_list* list)
{
    Cent_type_field* p = list->head;
    while (p) {
        Cent_type_field* temp = p;
        p = p->next;
        Cent_type_field_destroy(temp);
        free(temp);
    }
}
