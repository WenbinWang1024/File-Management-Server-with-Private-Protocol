#ifndef COMMANDS_H
#define COMMANDS_H

#include "includings.h"
#include "file_transfer.h"
#include "thread_pool.h"

#define MAX_CMD_NO 7

typedef enum {INVALID = -1, EMPTY, CD, LS, PUTS, GETS, REMOVE, PWD} CMD_T;

CMD_T get_cmd_type(char ** cmd_list, const char * cmd);

int analyze_cmd(pTrain_t pTrain, int fd, char * path, pThread_Pool_t pThread_Pool);

int cmd_cd(int fd, char * cmd, char * path);

int cmd_ls(int fd, char * cmd, char * path);

int cmd_puts(int fd, char * path);

int cmd_gets(int fd, char * cmd);

int cmd_rm(char * cmd);

int cmd_pwd(int fd, char * path);

#endif // COMMANDS_H
