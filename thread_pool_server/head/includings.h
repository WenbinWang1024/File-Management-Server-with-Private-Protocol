#ifndef INCLUDINGS_H
#define INCLUDINGS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#define ARGS_CHECK(argc, val) {if(argc != val){printf("args error!\n"); exit(-1);}}
#define ERROR_CHECK(ret, val, info) {if(ret == val){perror(info); exit(-1);}}

#define THREAD_ERRORCHECK(ret,funcName)  do{if(0 != ret) {printf("%s: %s\n",funcName,strerror(ret));}}while(0)
#define THREAD_ERROR_CHECK(ret,funcName)  do{if(0 != ret) {printf("%s: %s\n",funcName,strerror(ret));}}while(0)
#define CHILD_THREAD_ERROR_CHECK(ret,funcName)  do{if(0 != ret) {printf("%s: %s\n",funcName,strerror(ret)); return (void*)-1;}}while(0)

#endif // INCLUDINGS_H
