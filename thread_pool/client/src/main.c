#include "../head/commands.h"
#include "../head/epoll_management.h"
#include "../head/tcp_net_socket.h"
#include "../head/file_transfer.h"

int main(int argc, char ** argv) {
    // arg format: ./client [ip] [port]
    ARGS_CHECK(argc, 3);

    /* init zone */
    int ret = 0;
    // tcp connect
    int sfd = tcp_connect(argv[1], atoi(argv[2]));
    // print like bash
    printf("client > ");
    // command receiving buffer
    const int MAX_CMD_LEN = 1 << 10;
    char cmd[MAX_CMD_LEN] = {0};

    /* epoll init zone */
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    epoll_add(epfd, sfd);
    epoll_add(epfd, STDIN_FILENO);
    const int MAX_EVENT_NO = 3;
    struct epoll_event event_list[MAX_EVENT_NO];
    memset(event_list, 0, sizeof(event_list));

    while (1) {
        fflush(stdout);

        int prepared_num = epoll_wait(epfd, event_list, MAX_EVENT_NO, 0);
        for (int i = 0; i < prepared_num; ++i) {
            if (STDIN_FILENO == event_list[i].data.fd) {
                memset(cmd, 0, sizeof(cmd));
                ret = read(STDIN_FILENO, cmd, sizeof(cmd));
                ERROR_CHECK(ret, -1, "read");
                cmd[strlen(cmd) - 1] = '\0';

                /* analyze cmd */
                analyze_cmd(cmd, sfd);
                printf("client > ");
            } // if
            if (sfd == event_list[i].data.fd) {
                // detect disconnection from server
                char flg = 0;
                if (0 == recv(sfd, &flg, sizeof(flg), 0)) {
                    printf("\nUnexpected disconnection!\n");
                    exit(0);
                }
            } // if
        } // for
    } // while

    close(sfd);
    return 0;
}
