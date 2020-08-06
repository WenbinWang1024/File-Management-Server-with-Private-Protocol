#include "../head/work_queue.h"

int queue_init(pQueue_t pQueue) {
    memset(pQueue, 0, sizeof(queue_t));
    pthread_mutex_init(&pQueue->mutex, NULL);
    pthread_cond_init(&pQueue->cond, NULL);

    return 0;
}

int queue_push(pQueue_t pQueue, pNode_t pNew) {
    if (NULL == pQueue->pTail) {
        pQueue->pHead = pNew;
    }
    else {
        pQueue->pTail->pNext = pNew;
    }
    pQueue->pTail = pNew;
    ++pQueue->q_size;

    return 0;
}

int queue_pop(pQueue_t pQueue, pNode_t * ppFront) {
    if (NULL == pQueue->pHead) {
        return -1;
    }
    *ppFront = pQueue->pHead;
    pQueue->pHead = pQueue->pHead->pNext;
    --pQueue->q_size;
    if (0 == pQueue->q_size) {
        pQueue->pTail = NULL;
    }

    return 0;
}
