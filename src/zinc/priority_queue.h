#ifndef ZINC_PRIORITY_QUEUE_H
#define ZINC_PRIORITY_QUEUE_H

typedef struct Zinc_priority_task {
    int priority;
    void* data;
    struct Zinc_priority_task* next;
    struct Zinc_priority_task* prev;
} Zinc_priority_task;

typedef struct Zinc_priority_queue {
    Zinc_priority_task* head;
    Zinc_priority_task* tail;
} Zinc_priority_queue;

typedef void (*Zinc_priority_queue_func)(void* data);

void Zinc_priority_task_init(Zinc_priority_task *task);
void Zinc_priority_task_create(Zinc_priority_task** task);
void Zinc_priority_queue_init(Zinc_priority_queue *q);
void Zinc_priority_queue_create(Zinc_priority_queue** q);
void Zinc_priority_queue_add(Zinc_priority_queue *pq, Zinc_priority_task *task);
void Zinc_priority_queue_map(Zinc_priority_queue *q, Zinc_priority_queue_func func);
void Zinc_priority_queue_destroy(Zinc_priority_queue *q);

#endif