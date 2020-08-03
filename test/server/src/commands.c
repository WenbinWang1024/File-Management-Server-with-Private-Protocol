#include "../head/commands.h"

CMD_NO get_cmd_no(char ** cmd_list, int max_cmd_no, const char * cmd) {
    if (0 == strlen(cmd)) {
        return EMPTY;
    }
    for (int cmd_no = 1; cmd_no < max_cmd_no; ++cmd_no) {
        if (0 == strncmp(cmd_list[cmd_no], cmd, strlen(cmd_list[cmd_no]))) {
            return (CMD_NO) cmd_no;
        }
    }
    return INVALID;
}

int cmd_cd(int client_fd, const cmd_t * cmd) {
    int pos = 0;
    while (' ' != cmd->cmd[pos]) {
        ++pos;
    }
    while (' ' == cmd->cmd[pos]) {
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
