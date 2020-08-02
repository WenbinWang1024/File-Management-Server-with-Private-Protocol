#ifndef COMMANDS_H
#define COMMANDS_H

#include "includings.h"

typedef enum {INVALID = -1, EMPTY, CD, LS, PUTS, GETS, REMOVE, PWD} CMD_NO;

CMD_NO get_cmd_no(char ** cmd_list, int max_cmd_no, const char * cmd);

int cmd_pwd(int sfd, char * buf, int buf_size);

#endif // COMMANDS_H
