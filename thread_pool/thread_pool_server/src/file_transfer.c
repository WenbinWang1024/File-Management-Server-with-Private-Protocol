#include "../head/file_transfer.h"

int trans_file(int client_fd, const char * f_name) {
   
    train_t train_dir;
    memset(&train_dir, 0, sizeof(train_dir));
    int ret = 0;
    char path[MAX_PATH_LEN] = {0};
    sprintf(path, "%s%s", "./", f_name);
    // check file
    int fd = open(path, O_RDWR);
    //int has_file = 1;
    //printf("fd=%d",fd);
    if (fd == -1) {
        // file not exists
        // send error info
        //has_file = 0;
        //ret=send(client_fd, &has_file, sizeof(has_file), 0);
        printf("file not exists\n");
        return -1;
        
    }
    else {
        //ret=send(client_fd, &has_file, sizeof(has_file), 0);
        printf("check  succeed\n");
    }
    //path
    
    //printf("Path: %s\n", path);
    // send f_name to client
    train_dir.length = strlen(path);
    strcpy(train_dir.buf, path);
    ret = send(client_fd, &train_dir, sizeof(train_dir.length) + train_dir.length, 0);
    //printf("ret = %ld\n",sizeof(train_dir.length));
    ERROR_CHECK(ret, -1, "send");

    // send file length
    struct stat f_info;
    memset(&f_info, 0, sizeof(f_info));
    fstat(fd, &f_info);
    train_dir.length = sizeof(f_info.st_size);
    memcpy(train_dir.buf, &f_info.st_size, train_dir.length);
    ret = send(client_fd, &train_dir, sizeof(train_dir.length) + train_dir.length, 0);
    ERROR_CHECK(ret, -1, "send");

    // send file
    while (1) {
        ret = read(fd, train_dir.buf, sizeof(train_dir.buf));
        ERROR_CHECK(ret, -1, "read");

        train_dir.length = ret;
        ret = send(client_fd, &train_dir, sizeof(train_dir.length) + train_dir.length, 0);
        ERROR_CHECK(ret, -1, "send");
        //printf("ret = %d\n",ret);
        if (0 == train_dir.length) {
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
