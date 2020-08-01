#include "../head/epoll_management.h"
#include "../head/tcp_net_socket.h"

int main(int argc, char ** argv) {
    // ./ftpserver ../conf/server.conf
    ARGS_CHECK(argc, 2);

    // buffer
    char buf[1 << 10] = {0};
    
    // return val for checking
    int ret = 0;

    // tcp init
    int conf_fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(conf_fd, -1, "open conf_fd");
    read(conf_fd, buf, sizeof(buf));
    char ip[16] = {0};
    char port[5] = {0};
    sscanf(buf, "%s%s", ip, port);
    int sfd = tcp_init(ip, atoi(port));
    ERROR_CHECK(sfd, -1, "tcp_init");
    
    // client info
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = -1;

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
            }
            if (client_fd == event_list[i].data.fd) {
                memset(buf, 0, sizeof(buf));
                ret = recv(client_fd, buf, sizeof(buf), 0);
                if (0 == ret) {
                    printf("Client %s:%d disconnected!\n",
                           inet_ntoa(client_addr.sin_addr),
                           ntohs(client_addr.sin_port)
                          );
                    epoll_del(epfd, client_fd);
                    close(client_fd);
                }
                
                // analyze commands
                // test pwd
                if (0 == strcmp("pwd", buf)) {
                    char * wd = getcwd(NULL, 0);
                    send(client_fd, wd, strlen(wd), 0);
                }
            }
        }
    }
    
    close(client_fd);
    close(sfd);
    return 0;
}

