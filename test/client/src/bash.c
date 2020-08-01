#include "../head/epoll_management.h"
#include "../head/tcp_net_socket.h"

int main(int argc, char ** argv) {
    // ./bash [ip] [port]
    ARGS_CHECK(argc, 3);
    int ret = -1;

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
    printf("Server %s:%d successfully connected!\n", 
           inet_ntoa(server_addr.sin_addr),
           ntohs(server_addr.sin_port)
           );

    return 0;
}
