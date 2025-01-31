#include "priority_queue.h"
#include <stdlib.h>
#include "memory.h"

void Zinc_priority_task_init(Zinc_priority_task *task)
{
    task->priority = 0;
    task->data = NULL;
    task->next = NULL;
    task->prev = NULL;
}

void Zinc_priority_task_create(Zinc_priority_task** task)
{
    Zinc_malloc_safe((void**)task, sizeof(Zinc_priority_task));
    Zinc_priority_task_init(*task);
}

void Zinc_priority_queue_init(Zinc_priority_queue *q)
{
    q->head = NULL;
    q->tail = NULL;
}

void Zinc_priority_queue_create(Zinc_priority_queue** q)
{
    Zinc_malloc_safe((void**)q, sizeof(Zinc_priority_queue));
    Zinc_priority_queue_init(*q);
}

void Zinc_priority_queue_add(Zinc_priority_queue *q, Zinc_priority_task *task)
{
    if (q->head && q->tail) {
        Zinc_priority_task *p = q->tail;
        while (p) {
            if (task->priority >= p->priority) {
                break;
            }
            p = p->prev;
        }

        if (p) {
            task->next = p->next;
            p->next = task;
            task->prev = p;
        } else {
            task->next = q->head;
            q->head->prev = task;
            q->head = task;
        }
    } else {
        q->head = task;
        q->tail = task;
    }
}

void Zinc_priority_queue_map(Zinc_priority_queue *q, Zinc_priority_queue_func func)
{
    Zinc_priority_task* p = q->head;
    while (p) {
        func(p->data);
        p = p->next;
    }
}

void Zinc_priority_queue_destroy(Zinc_priority_queue *q)
{
    Zinc_priority_task* p = q->head;
    while (p) {
        Zinc_priority_task* temp = p;
        p = p->next;
        free(temp);
    }
}