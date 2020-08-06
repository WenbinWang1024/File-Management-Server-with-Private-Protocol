#include "../head/commands.h"

CMD_T get_cmd_type(char ** cmd_list, const char * cmd) {
    if (0 == strlen(cmd)) {
        return EMPTY;
    }
    for (int i = 1; i < MAX_CMD_NO; ++i) {
        if (0 == strncmp(cmd_list[i], cmd, strlen(cmd_list[i]))) {
            return (CMD_T) i;
        }
    }
    
    return INVALID;
}

int analyze_cmd(char * cmd, int fd) {
    char * cmd_list[MAX_CMD_NO] = {
        "", "cd", "ls", "puts", "gets", "remove", "pwd"
    };

    CMD_T cmd_type = get_cmd_type(cmd_list, cmd);
    switch(cmd_type) {
    case EMPTY:
        break;
    case CD:
        cmd_cd(fd, cmd);
        break;
    case LS:
        cmd_ls(fd, cmd);
        break;
    case PUTS:
        break;
    case GETS:
        break;
    case REMOVE:
        cmd_rm(fd, cmd);
        break;
    case PWD:
        cmd_pwd(fd, cmd);
        break;
    default:
        printf("Invalid command\n");
        break;
    }

    return 0;
}


int cmd_cd(int fd, const char * cmd) {
    train_t train;
    memset(&train, 0, sizeof(train));

    train.length = strlen(cmd);
    memcpy(train.buf, cmd, strlen(cmd));
    send(fd, &train, sizeof(train.length) + train.length, 0);

    char buf[1 << 10] = {0};
    recv(fd, buf, sizeof(buf), 0);
    printf("%s\n", buf);

    return 0;
}

int cmd_ls(int fd, const char * cmd) {
    train_t train;
    memset(&train, 0, sizeof(train));

    train.length = strlen(cmd);
    memcpy(train.buf, cmd, strlen(cmd));
    send(fd, &train, sizeof(train.length) + train.length, 0);

    while (1) {
        memset(&train, 0, sizeof(train));
        recv(fd, &train, sizeof(train), 0);

        if (0 == train.length) {
            break;
        }
        printf("%s", train.buf);
    }

    return 0;
}

int cmd_rm(int fd, const char * cmd) {
    int pos = 0;
    while (pos < strlen(cmd) && ' ' != cmd[pos])
        ++pos;
    while (pos < strlen(cmd) && ' ' == cmd[pos])
        ++pos;
    if ('/' == cmd[pos]) {
        printf("Permission denied!\n");
        return -1;
    }

    train_t train;
    memset(&train, 0, sizeof(train));

    train.length = strlen(cmd);
    memcpy(train.buf, cmd, strlen(cmd));
    send(fd, &train, sizeof(train.length) + train.length, 0);

    return 0;
}

int cmd_pwd(int fd, const char * cmd) {
    train_t train;
    memset(&train, 0, sizeof(train));

    train.length = strlen(cmd);
    memcpy(train.buf, cmd, strlen(cmd));
    send(fd, &train, sizeof(train.length) + train.length, 0);

    char path[MAX_PATH_LEN] = {0};
    recv(fd, path, sizeof(path), 0);
    printf("%s\n", path);

    return 0;
}
