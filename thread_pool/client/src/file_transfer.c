#include "../head/file_transfer.h"

int trans_file(int client_fd, const char * f_name) {
    train_t train;
    memset(&train, 0, sizeof(train));
    int ret = -1;

    // send f_name to client
    train.length = strlen(f_name);
    strcpy(train.buf, f_name);
    ret = send(client_fd, &train, sizeof(train.length) + train.length, 0);
    ERROR_CHECK(ret, -1, "send");

    // path
    char path[MAX_PATH_LEN] = {0};
    sprintf(path, "%s%s", "./", f_name);
    printf("Path: %s\n", path);

    // check file
    int fd = open(path, O_RDWR);
    int has_file = 1;
    if (-1 == fd) {
        // file not exists
        // send error info
        has_file = 0;
        send(client_fd, &has_file, sizeof(has_file), 0);
        return -1;
    }
    else {
        send(client_fd, &has_file, sizeof(has_file), 0);
    }

    // send file length
    struct stat f_info;
    memset(&f_info, 0, sizeof(f_info));
    fstat(fd, &f_info);
    train.length = sizeof(f_info.st_size);
    memcpy(train.buf, &f_info.st_size, train.length);
    ret = send(client_fd, &train, sizeof(train.length) + train.length, 0);
    ERROR_CHECK(ret, -1, "send");

    // send file
    while (1) {
        ret = read(fd, train.buf, sizeof(train.buf));
        ERROR_CHECK(ret, -1, "read");

        train.length = ret;
        ret = send(client_fd, &train, sizeof(train.length) + train.length, 0);
        ERROR_CHECK(ret, -1, "send");

        if (0 == train.length) {
            break;
        }
    }

    close(fd);
    return 0;
}

int cycle_recv(int fd, void * buf, size_t data_length) {
    size_t len = 0;
    size_t total_len = 0;
    char * ptr = (char *) buf;
    while (total_len < data_length) {
        len = recv(fd, ptr + total_len, data_length - total_len, 0);
        total_len += len;
        if (0 == len) {
            return -1;
        }
    }

    return 0;
}

int file_puts(int client_fd,char *filename){//上传
    int fd = open(filename,O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    train_t train;
    memset(&train,0,sizeof(train));
    int ret = 0;

    //发送文件名
    train.length = strlen(filename);
    strcpy(train.buf,filename);
    send(client_fd,&train,sizeof(train.length)+train.length,0);
    ERROR_CHECK(ret,-1,"send filename");

    //发送文件大小
    struct stat fileInfo;
    memset(&fileInfo,0,sizeof(fileInfo));
    fstat(fd,&fileInfo);
    train.length = sizeof(fileInfo.st_size);
    memcpy(train.buf,&fileInfo.st_size,train.length);
    ret  = send(client_fd,&train,sizeof(train.length)+train.length,0);
    /* ret = send(client_fd,&train.buf,train.length,0); */
    ERROR_CHECK(ret,-1,"send filesize");
    /* printf("%ld %ld\n",train.length,fileInfo.st_size); */

    //读取文件内容并发送
    while(1){
        ret = read(fd,train.buf,sizeof(train.buf));
        ERROR_CHECK(ret,-1,"read");
        train.length = ret;

        if(train.length == 0){
            break;//发送文件完成，跳出循环
        }

        send(client_fd,&train,sizeof(train.length)+train.length,0);
        ERROR_CHECK(ret,-1,"send file");
    }

    //发送文件发送完成的结束标志
    ret = send(client_fd,&train,sizeof(train.length)+train.length,0);
    ERROR_CHECK(ret,-1,"send fileFinish");

    printf("send success\n");

    close(fd);
    return 0;
}

