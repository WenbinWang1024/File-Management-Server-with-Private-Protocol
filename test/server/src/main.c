#include "../head/epoll_management.h"
#include "../head/tcp_net_socket.h"
#include "../head/commands.h"

int main(int argc, char ** argv) {
    // arg format: ./ftpserver ../conf/server.conf
    ARGS_CHECK(argc, 2);

    /* init zone */
    // buffer
    char buf[1 << 10] = {0};
    // cmd
    cmd_t cmd;
    // return val for checking
    int ret = 0;
    // tcp regist
    int sfd = tcp_regist(argv[1]);    
    // client info
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = -1;

    /* epoll init zone */
    // epoll handles
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    // epoll events
    epoll_add(epfd, sfd);
    // epoll event list
    const int MAX_EVENT_NO = 3;
    struct epoll_event event_list[MAX_EVENT_NO];
    memset(event_list, 0, sizeof(event_list));

    while (1) {
        int prepared_num = epoll_wait(epfd, event_list, MAX_EVENT_NO, 0);
        for (int i = 0; i < prepared_num; ++i) {
            if (sfd == event_list[i].data.fd) {
                client_fd = accept(sfd, (struct sockaddr *) &client_addr, &addr_len);
                ERROR_CHECK(client_fd, -1, "accept");
                printf("Client %s:%d successfully connected!\n",
                       inet_ntoa(client_addr.sin_addr),
                       ntohs(client_addr.sin_port)
                       );
                epoll_add(epfd, client_fd);
            } // if
            if (client_fd == event_list[i].data.fd) {
                memset(&cmd, 0, sizeof(cmd));
                ret = recv(client_fd, &cmd, sizeof(cmd), 0);
                if (0 == ret) {
                    printf("Client %s:%d disconnected!\n",
                           inet_ntoa(client_addr.sin_addr),
                           ntohs(client_addr.sin_port)
                          );
                    epoll_del(epfd, client_fd);
                    close(client_fd);
                } // if

                /* analyze commands */
                switch(cmd.cmd_no) {
                case CD:
                    cmd_cd(client_fd, &cmd);
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
                    cmd_pwd(client_fd);
                    break;
                default:
                    continue;   
                }
                if (0 == strncmp("cd", buf, 2)) {

                }
                else if (0 == strcmp("pwd", buf)) {
                    cmd_pwd(client_fd);
                } // if
                
            } // if
        } // for
    } // while
    
    close(client_fd);
    close(sfd);
    return 0;
}

