#include "../head/commands.h"

int cmd_cd(int client_fd, const cmd_t * cmd) {
    int pos = 0;
    while (pos < strlen(cmd->cmd) && ' ' != cmd->cmd[pos]) {
        ++pos;
    }
    while (pos < strlen(cmd->cmd) && ' ' == cmd->cmd[pos]) {
        ++pos;
    }

    int ret = chdir(cmd->cmd + pos);
    if (-1 == ret) {
        char error[] = "error: No such file or directory";
        ERROR_CHECK(send(client_fd, error, strlen(error), 0), -1, "send");
    }
    else {
        cmd_pwd(client_fd);
    }

    return 0;
}

int cmd_pwd(int client_fd) {
    char * wd = getcwd(NULL, 0);
    ERROR_CHECK(send(client_fd, wd, strlen(wd), 0), -1, "send"); 
    
    return 0;
}
