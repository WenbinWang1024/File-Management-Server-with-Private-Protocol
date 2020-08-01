#ifndef TCP_NET_SOCKET_H
#define TCP_NET_SOCKET_H

#include "includings.h"

int tcp_init(const char * ip, int port);

int tcp_accept(int sfd);

int tcp_connect(const char * ip, int port);

#endif // TCP_NET_SOCKET_H
