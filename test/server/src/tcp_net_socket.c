#include "../head/tcp_net_socket.h"

int tcp_init(const char * ip, int port) {
    // socket
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    // server_addr
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    
    // reuse ip and port
    int reuse = 1;
    ERROR_CHECK(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)), -1, "setsockopt");

    // bind
    ERROR_CHECK(bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)), -1, "bind");

    // listen
    ERROR_CHECK(listen(sfd, 10), -1, "listen");

    // return
    return sfd;
}

int tcp_regist(const char * server_conf) {
    int conf_fd = open(server_conf, O_RDONLY);
    ERROR_CHECK(conf_fd, -1, "open");

    char temp_buf[1 << 5] = {0};
    char ip[16] = {0};
    char port[5] = {0};
    read(conf_fd, temp_buf, sizeof(temp_buf));
    sscanf(temp_buf, "%s%s", ip, port);

    close(conf_fd);
    int sfd = tcp_init(ip, atoi(port));
    return sfd;
}

int tcp_accept(int sfd) {
    // client_addr
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t addr_len = sizeof(client_addr);

    // connect
    int client_fd = accept(sfd, (struct sockaddr *) &client_addr, &addr_len);
    ERROR_CHECK(client_fd, -1, "accept");
    printf("Client %s:%d successfully connected!\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port)
           );

    // return
    return client_fd;
}

int tcp_connect(const char * ip, int port) {
    // socket
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    // server_addr
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    
    // connect
    ERROR_CHECK(connect(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)), -1, "connect");

    // return
    return sfd;
}
