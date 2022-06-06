#include "allocator.h"
#include "result.h"
#include "memory.h"

void allocator_init(struct allocator* al)
{
	al->head = NULL;
	al->tail = NULL;
}

enum result allocator_malloc(struct allocator* al, void** buf, size_t size)
{
	enum result r;
	struct allocator_node* aln;

	r = malloc_safe(buf, size);
	if (r == result_error) {
		return r;
	}

	r = malloc_safe(&aln, sizeof(struct allocator_node));
	if (r == result_error) {
		free(*buf);
		return r;
	}

	aln->p = *buf;
	aln->next = al->head;
	al->head = aln;
	return result_ok;
}

struct allocator_node* allocator_find(struct allocator* al, void* p)
{
	for (struct allocator_node* aln = al->head; aln; aln = aln->next) {
		if (aln->p == p) {
			return aln;
		}
	}
	return NULL;
}

enum result allocator_realloc(struct allocator* al, void** buf, size_t size)
{
	enum result r;
	struct allocator_node* aln;

	r = realloc_safe(buf, size);
	if (r == result_error) {
		return r;
	}

	aln = allocator_find(al, *buf);
	if (!aln) {
		free(*buf);
		return set_error("pointer not found in allocator");
	}
	aln->p = *buf;

	return result_ok;
}

void allocator_destroy(struct allocator* al)
{
	struct allocator_node* aln = al->head;
	struct allocator_node* temp;
	while (aln) {
		free(aln->p);
		temp = aln;
		aln = aln->next;
		free(temp);
	}
	allocator_init(al);
}

void allocator_transfer(struct allocator* src, struct allocator* dest)
{
	struct allocator_node* aln1;
	struct allocator_node* aln2;
	aln1 = src->tail;
	aln2 = dest->head;
	if (aln1) {
		aln1->next = aln2;
	}
	if (aln2) {
		aln2->prev = aln1;
	}
	dest->head = src->head;
	if (!dest->tail) {
		dest->tail = src->tail;
	}
	allocator_init(src);
}

struct allocator_node* allocator_remove(struct allocator* al, void* p)
{
	struct allocator_node* prev;
	struct allocator_node* next;
	for (struct allocator_node* n = al->head; n; n = n->next) {
		if (n->p == p) {
			prev = n->prev;
			next = n->next;
			if (prev) {
				prev->next = next;
			}
			if (next) {
				next->prev = prev;
			}
			if (al->head == n) {
				if (prev) {
					al->head = prev;
				} else {
					al->head = next;
				}
			}
			if (al->tail == n) {
				if (next) {
					al->tail = next;
				} else {
					al->tail = prev;
				}
			}
			return n;
		}
	}
	return NULL;
}

void allocator_add(struct allocator* al, struct allocator_node* n)
{
	struct allocator_node* temp = al->head;

	temp->prev = n;
	
	n->next = temp;
	n->prev = NULL;

	al->head = n;
	if (!al->tail) {
		al->tail = n;
	}
}

void allocator_transfer_item(struct allocator* src, struct allocator* dest, void* p)
{
	struct allocator_node* n = NULL;

	n = allocator_remove(src, p);

	if (n) {
		allocator_add(dest, n);
	}
}

void allocator_destroy_item(struct allocator* al, void* p)
{
	struct allocator_node* n = allocator_remove(al, p);

	if (n) {
		free(n->p);
		free(n);
	}
}
