#include "../head/epoll_management.h"
#include "../head/tcp_net_socket.h"
#include "../head/commands.h"
#include "../head/str_dealing.h"

int main(int argc, char ** argv) {
    // ./bash [ip] [port]
    ARGS_CHECK(argc, 3);
    int ret = -1;

    // commands
    const int MAX_CMD_NO = 7;
    char * cmd_list[MAX_CMD_NO] = {
        "", "cd", "ls", "puts", "gets", "remove", "pwd"
    };

    // buffer
    char buf[1 << 10] = {0};

    // socket and connect
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  // ip
    server_addr.sin_port = htons(atoi(argv[2]));       // port
    ret = connect(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "connect");
    system("clear");
    printf("Server %s:%d successfully connected!\n", 
           inet_ntoa(server_addr.sin_addr),
           ntohs(server_addr.sin_port)
          );
    printf("client > ");

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

    /* int input_not_empty = 0; */

    while (1) {
        fflush(stdout);

        int prepared_num = epoll_wait(epfd, event_list, MAX_EVENT_NO, 0);
        for (int i = 0; i < prepared_num; ++i) {
            if (STDIN_FILENO == event_list[i].data.fd) {
                memset(buf, 0, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read");
                pretreat_str(buf, strlen(buf));

                /* input_not_empty = strlen(buf); */
                /* if (!input_not_empty) { */
                /*     printf("client > "); */
                /*     continue; */
                /* } */

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
        } // for
    } // while

    close(sfd);
    return 0;
}
