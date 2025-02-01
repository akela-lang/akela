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

void Zinc_priority_queue_init(Zinc_priority_queue *pq)
{
    pq->head = NULL;
    pq->tail = NULL;
}

void Zinc_priority_queue_create(Zinc_priority_queue** pq)
{
    Zinc_malloc_safe((void**)pq, sizeof(Zinc_priority_queue));
    Zinc_priority_queue_init(*pq);
}

void Zinc_priority_queue_add(Zinc_priority_queue *pq, Zinc_priority_task *task)
{
    if (pq->head && pq->tail) {
        Zinc_priority_task *p = pq->tail;
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
            if (task->next) {
                task->next->prev = task;
            } else {
                pq->tail = task;
            }
        } else {
            task->next = pq->head;
            pq->head->prev = task;
            pq->head = task;
        }
    } else {
        pq->head = task;
        pq->tail = task;
    }
}

Zinc_priority_task* Zinc_priority_queue_pop(Zinc_priority_queue *pq)
{
    if (pq->tail) {
        Zinc_priority_task *p = pq->tail;
        if (p->prev) {
            pq->tail = p->prev;
            pq->tail->next = NULL;
        } else {
            pq->head = NULL;
            pq->tail = NULL;
        }

        p->next = NULL;
        p->prev = NULL;
        return p;
    }

    return NULL;
}

void Zinc_priority_queue_map(Zinc_priority_queue *pq, Zinc_priority_queue_func func)
{
    Zinc_priority_task* p = pq->head;
    while (p) {
        func(p->data);
        p = p->next;
    }
}

void Zinc_priority_queue_destroy(Zinc_priority_queue *pq)
{
    Zinc_priority_task* p = pq->head;
    while (p) {
        Zinc_priority_task* temp = p;
        p = p->next;
        free(temp);
    }
}