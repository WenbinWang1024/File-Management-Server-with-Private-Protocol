#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include "includings.h"

typedef struct node {
    int client_fd;
    char filename[NAME_MAX];
    struct node * pNext;
} node_t, * pNode_t;

typedef struct {
    pNode_t pHead;
    pNode_t pTail;
    size_t q_size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} queue_t, * pQueue_t;

int queue_init(pQueue_t pQueue);

int queue_push(pQueue_t pQueue, pNode_t pNew);

int queue_pop(pQueue_t pQueue, pNode_t * ppFront);

#endif // WORK_QUEUE_H
