#include "../head/tcp_net_socket.h"
#include "../head/work_queue.h"
#include "../head/file_transfer.h"
#include "../head/epoll_management.h"
#include "../head/thread_pool.h"

int main(int argc, char ** argv) {
    // arg format: ./ftpserver ../conf/server.conf
    ARGS_CHECK(argc, 2);

    /* init zone */
    int ret = 0;
    // current path
    char * cur_path = getcwd(NULL, 0);
    ERROR_CHECK(cur_path, NULL, "getcwd");
    // tcp regist
    int sfd = tcp_regist(argv[1]);
    // client info
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = -1;
    // thread settings
    const int MAX_THREAD_NO = 3;
    pThread_Pool_t pThread_Pool = (pThread_Pool_t) calloc(1, sizeof(thread_pool_t));
    thread_pool_init(pThread_Pool, MAX_THREAD_NO);
    thread_pool_start(pThread_Pool);

    /* epoll init zone */
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    epoll_add(epfd, sfd);
    const int MAX_EVENT_NO = 3;
    struct epoll_event event_list[MAX_EVENT_NO];
    memset(event_list, 0, sizeof(event_list));

    while (1) {
        int prepared_num = epoll_wait(epfd, event_list, MAX_EVENT_NO, 0);
        for (int i = 0; i < prepared_num; ++i) {
            if (sfd == event_list[i].data.fd) {
                client_fd = tcp_accept(sfd);
                epoll_add(epfd, client_fd);
            } // if
            if (client_fd == event_list[i].data.fd) {
                train_t train;
                memset(&train, 0, sizeof(train));
                ret = cycle_recv(client_fd, &train.length, sizeof(train.length));
                if (0 == train.length) {
                    epoll_del(epfd, client_fd);
                    continue;
                }
                ret = cycle_recv(client_fd, &train.buf, train.length);

                /* dealing cmds */

            } // if
        } // for
    } // while

    return 0;
}
