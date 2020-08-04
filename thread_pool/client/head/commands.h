#ifndef COMMANDS_H
#define COMMANDS_H

#include "includings.h"
#include "file_transfer.h"

#define MAX_CMD_NO 7

typedef enum {INVALID = -1, EMPTY, CD, LS, PUTS, GETS, REMOVE, PWD} CMD_T;

CMD_T get_cmd_type(char ** cmd_list, const char * cmd);

int analyze_cmd(char * cmd, int fd);

int cmd_cd(int fd, const char * cmd);

int cmd_pwd(int fd, const char * cmd);

#endif // COMMANDS_H
