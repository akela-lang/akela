#ifndef _DEFER_H
#define _DEFER_H

struct defer_node {
	void (*f)(void*);
	void* d;
	struct defer_node* below;
};

enum result defer(void (*f)(void*), void* d, struct defer_node** stack);
void cleanup(struct defer_node* stack);
void cleanup_stack(struct defer_node* stack);

#endif
