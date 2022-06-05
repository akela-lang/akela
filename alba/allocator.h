#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include <stdlib.h>

struct allocator_node {
	void* p;
	struct allocator_node* next;
	struct allocator_node* prev;
};

struct allocator {
	struct allocator_node* head;
	struct allocator_node* tail;
};

void allocator_init(struct allocator* al);
enum result allocator_malloc(struct allocator* al, void** buf, size_t size);
struct allocator_node* allocator_find(struct allocator* al, void* p);
enum result allocator_realloc(struct allocator* al, void** buf, size_t size);
void allocator_destroy(struct allocator* al);
void allocator_transfer(struct allocator* src, struct allocator* dest);
struct allocator_node* allocator_remove(struct allocator* al, void* p);
void allocator_add(struct allocator* al, struct allocator_node* n);
void allocator_transfer_item(struct allocator* src, struct allocator* dest, void* p);
void allocator_destroy_item(struct allocator* al, void* p);

#endif
