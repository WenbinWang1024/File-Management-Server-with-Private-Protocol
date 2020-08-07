#ifndef EPOLL_MANAGEMENT_H
#define EPOLL_MANAGEMENT_H

#include "includings.h"

int epoll_add(int epfd, int fd);

int epoll_del(int epfd, int fd);

#endif // EPOLL_MANAGEMENT_H
