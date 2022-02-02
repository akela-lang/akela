#include "defer.h"
#include "result.h"
#include "memory.h"

enum result_enum defer(void (*f)(void*), void* d, struct defer_node** stack)
{
	struct defer_node* n;
	enum result_enum r = malloc_safe(&n, sizeof(struct defer_node));
	if (r == error_result) {
		return r;
	}
	n->f = f;
	n->d = d;
	n->below = *stack;
	*stack = n;
	return ok_result;
}

void cleanup(struct defer_node* stack)
{
	struct defer_node* p = stack;
	while (p) {
		p->f(p->d);
		struct defer_node* temp = p;
		p = p->below;
		free(temp);
	}

}

void cleanup_stack(struct defer_node* stack)
{
	struct defer_node* p = stack;
	while (p) {
		struct defer_node* temp = p;
		p = p->below;
		free(temp);
	}

}
