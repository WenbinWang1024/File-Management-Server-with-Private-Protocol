#ifndef COMMANDS_H
#define COMMANDS_H

#include "includings.h"

typedef enum {INVALID = -1, EMPTY, CD, LS, PUTS, GETS, REMOVE, PWD} CMD_NO;

typedef struct {
    CMD_NO cmd_no;
    char cmd[1 << 7];
} cmd_t, * pCmd_t;

CMD_NO get_cmd_no(char ** cmd_list, int max_cmd_no, const char * cmd);

int cmd_cd(int sfd, const cmd_t * cmd, char * buf, int buf_size);

int cmd_pwd(int sfd, const cmd_t * cmd, char * buf, int buf_size);

#endif // COMMANDS_H
