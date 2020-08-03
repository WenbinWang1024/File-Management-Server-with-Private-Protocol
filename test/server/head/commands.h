#ifndef COMMANDS_H
#define COMMANDS_H

#include "includings.h"

typedef enum {INVALID = -1, EMPTY, CD, LS, PUTS, GETS, REMOVE, PWD} CMD_NO;

typedef struct {
    CMD_NO cmd_no;
    char cmd[1 << 7];
} cmd_t, * pCmd_t;

CMD_NO get_cmd_no(char ** cmd_list, int max_cmd_no, const char * cmd);

int cmd_cd(int client_fd, const cmd_t * cmd);

int cmd_pwd(int client_fd);

#endif
