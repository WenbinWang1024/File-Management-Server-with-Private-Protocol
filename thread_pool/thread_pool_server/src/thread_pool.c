#include "../head/thread_pool.h"

int thread_pool_init(pThread_Pool_t pThread_Pool, size_t thread_num) {
    memset(pThread_Pool, 0, sizeof(thread_pool_t));
    pThread_Pool->pThid = (pthread_t *) calloc(thread_num, sizeof(pthread_t));
    pThread_Pool->thread_num = thread_num;
    queue_init(&pThread_Pool->que);

    return 0;
}

int thread_pool_start(pThread_Pool_t pThread_Pool) {
    if (!pThread_Pool->start_flg) {
        for (int i = 0; i < pThread_Pool->thread_num; ++i) {
            pthread_create(pThread_Pool->pThid + i, NULL, thread_func, (void *) pThread_Pool);
        }
        pThread_Pool->start_flg = 1;
    }

    return 0;
}

void * thread_func(void * pArg) {
    pThread_Pool_t pThread_Pool = (pThread_Pool_t) pArg;
    pNode_t pGet = NULL;

    int ret = 0;

    while (1) {
        pthread_mutex_lock(&pThread_Pool->que.mutex);
        if (!pThread_Pool->que.q_size) {
            pthread_cond_wait(&pThread_Pool->que.cond, &pThread_Pool->que.mutex);
        }
        ret = queue_pop(&pThread_Pool->que, &pGet);
        pthread_mutex_unlock(&pThread_Pool->que.mutex);

        if (0 == ret) {
            trans_file(pGet->client_fd, pGet->f_name);
            close(pGet->client_fd);
            free(pGet);
            pGet = NULL;
        }
    }
    
    pthread_exit(NULL);
}
