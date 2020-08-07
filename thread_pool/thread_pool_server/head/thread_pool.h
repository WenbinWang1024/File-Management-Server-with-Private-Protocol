#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "includings.h"
#include "tcp_net_socket.h"
#include "work_queue.h"
#include "file_transfer.h"

typedef struct {
    queue_t que;
    pthread_t * pThid;
    size_t thread_num;
    int start_flg;
} thread_pool_t, * pThread_Pool_t;

int thread_pool_init(pThread_Pool_t pThread_Pool, size_t thread_num);

int thread_pool_start(pThread_Pool_t pThread_Pool);

void * thread_func(void * pArg);

#endif // THREAD_POOL_H
