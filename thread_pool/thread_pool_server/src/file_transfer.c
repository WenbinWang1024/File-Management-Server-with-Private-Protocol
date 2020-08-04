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
