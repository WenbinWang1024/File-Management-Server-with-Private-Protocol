#include "../head/epoll_management.h"

int epoll_add(int epfd, int fd) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = EPOLLIN;
    ERROR_CHECK(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event), -1, "epoll_add");

    return 0;
}
