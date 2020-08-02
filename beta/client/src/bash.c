#include "../head/epoll_management.h"
#include "../head/tcp_net_socket.h"
#include "../head/commands.h"
#include "../head/str_dealing.h"

int main(int argc, char ** argv) {
    // arg format: ./bash [ip] [port]
    ARGS_CHECK(argc, 3);

    // tcp connect
    int sfd = tcp_connect(argv[1], atoi(argv[2]));

    /* init zone */
    // register commands
    const int MAX_CMD_NO = 7;
    char * cmd_list[MAX_CMD_NO] = {
        "", "cd", "ls", "puts", "gets", "remove", "pwd"
    };
    // buffer
    char buf[1 << 10] = {0};
    // return val
    int ret = -1;
    // print like bash
    printf("client > ");

    /* epoll init zone */
    // epoll handles
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    // epoll events
    epoll_add(epfd, sfd);
    epoll_add(epfd, STDIN_FILENO);
    // epoll event list
    const int MAX_EVENT_NO = 3;
    struct epoll_event event_list[MAX_EVENT_NO];
    memset(event_list, 0, sizeof(event_list));

    while (1) {
        fflush(stdout);

        int prepared_num = epoll_wait(epfd, event_list, MAX_EVENT_NO, 0);
        for (int i = 0; i < prepared_num; ++i) {
            if (STDIN_FILENO == event_list[i].data.fd) {
                memset(buf, 0, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read");
                pretreat_str(buf, strlen(buf));

                CMD_NO cmd = get_cmd_no(cmd_list, MAX_CMD_NO, buf);
                switch (cmd) {
                case EMPTY:
                    break;
                case CD:
                    break;
                case LS:
                    break;
                case PUTS:
                    break;
                case GETS:
                    break;
                case REMOVE:
                    break;
                case PWD:
                    cmd_pwd(sfd, buf, sizeof(buf));
                    break;
                default:
                    // INVALID
                    printf("Invalid command\n");
                }
                printf("client > ");

            } // if

            if (sfd == event_list[i].data.fd) {
                if (0 == recv(sfd, buf, sizeof(buf), 0)) {
                    printf("\nUnexpected disconnection!\n");
                    exit(0);
                }
            }
        } // for
    } // while

    close(sfd);
    return 0;
}
