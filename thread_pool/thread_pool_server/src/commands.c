#include "../head/commands.h"

CMD_T get_cmd_type(char ** cmd_list, const char * cmd) {
    for (int i = 1; i < MAX_CMD_NO; ++i) {
        if (0 == strncmp(cmd_list[i], cmd, strlen(cmd_list[i]))) {
            return (CMD_T) i;
        }
    }
    
    return INVALID;
}

int analyze_cmd(pTrain_t pTrain, int fd, char * path, pThread_Pool_t pThread_Pool) {
    char * cmd_list[MAX_CMD_NO] = {
        "", "cd", "ls", "puts", "gets", "remove", "pwd"
    };

    CMD_T cmd_type = get_cmd_type(cmd_list, pTrain->buf);
    switch(cmd_type) {
    case CD:
        cmd_cd(fd, pTrain->buf, path);
        break;
    case LS:
        break;
    case PUTS:
        break;
    case GETS:
        break;
    case REMOVE:
        break;
    case PWD:
        cmd_pwd(fd, path);
        break;
    default:
        break;
    }

    return 0;
}

int cmd_cd(int fd, char * cmd, char * path) {
    int pos = 0;
    while (pos < strlen(cmd) && ' ' != cmd[pos])
        ++pos;
    while (pos < strlen(cmd) && ' ' == cmd[pos])
        ++pos;

    int ret = chdir(cmd + pos);
    if (-1 == ret) {
        char error_info[] = "error: No such file or directory";
        send(fd, error_info, strlen(error_info), 0);
    }
    else {
        char * wd = getcwd(cmd + pos, MAX_PATH_LEN);
        memset(path, 0, strlen(path));
        strcpy(path, wd);
        send(fd, path, strlen(path), 0);
    }

    return 0;
}

int cmd_pwd(int fd, char * path) {
    char * wd = getcwd(NULL, 0);
    memset(path, 0, strlen(path));
    strcpy(path, wd);
    send(fd, path, strlen(path), 0);

    return 0;
}
