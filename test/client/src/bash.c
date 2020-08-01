#include "../head/epoll_management.h"
#include "../head/tcp_net_socket.h"
#include "../head/str_dealing.h"

int main(int argc, char ** argv) {
    // ./bash [ip] [port]
    ARGS_CHECK(argc, 3);
    int ret = -1;

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

    int tolerance = 1;
    int input_not_empty = 0;

    while (1) {
        fflush(stdout);
        if (tolerance) {
            if (!input_not_empty) {
                printf("client > ");
            }
            tolerance = 0;
        }

        int prepared_num = epoll_wait(epfd, event_list, MAX_EVENT_NO, 0);
        for (int i = 0; i < prepared_num; ++i) {
            if (STDIN_FILENO == event_list[i].data.fd) {
                memset(buf, 0, sizeof(buf));
                fgets(buf, sizeof(buf), stdin);
                /* fflush(stdin); */
                pretreat_str(buf, strlen(buf));
                input_not_empty = send(sfd, buf, strlen(buf), 0);
                tolerance = 1;
            } // if
            if (sfd == event_list[i].data.fd) {
                memset(buf, 0, sizeof(buf));
                ret = recv(sfd, buf, sizeof(buf), 0);
                if (0 == ret) {
                    // server disconnected
                    printf("Unexpectedly disconnected!\n");
                    exit(0);
                } // if
                printf("%s", buf);
            } // if
        } // for
    } // while

    close(sfd);
    return 0;
}
