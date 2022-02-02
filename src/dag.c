#include <stdlib.h>
#include "dag.h"

void dag_init_node(struct dag_node* n)
{
	n->type = dag_type_none;
	string_init(&n->value);
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}
