#include "../head/commands.h"
#include "../head/file_info.h"

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
        cmd_ls(fd, pTrain->buf, path);
        break;
    case PUTS:
        file_puts(fd,path);
        break;
    case GETS:
        cmd_gets(fd, pTrain->buf);
        break;
    case REMOVE:
        cmd_rm(pTrain->buf);
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

int cmd_ls(int fd, char * cmd, char * path) {
    if (0 != strcmp(cmd, "ls")) {
        return -1;
    }

    DIR * dirp = opendir(path);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent * pDirent = NULL;

    char stat_ret[1 << 10] = {0};
    char stat_buf[1 << 10] = {0};

    struct stat buf;

    while (NULL != (pDirent = readdir(dirp))) {
        memset(&buf, 0, sizeof(buf));

        int ret = stat(pDirent->d_name, &buf);
        ERROR_CHECK(ret, -1, "stat");

        memset(stat_ret, 0, sizeof(stat_ret));
        get_file_type(buf.st_mode, stat_ret);
        get_file_perm(buf.st_mode, stat_ret);
        stat_ret[10] = ' ';

        memset(stat_buf, 0, sizeof(stat_buf));
        sprintf(stat_buf," %ld %s %s %5ld %20s %s",
                buf.st_nlink,
                getpwuid(buf.st_uid)->pw_name,
                getgrgid(buf.st_gid)->gr_name,
                buf.st_size,
                pDirent->d_name,
                ctime(&buf.st_mtime)
               );
        strcat(stat_ret, stat_buf);
        send(fd, stat_ret, strlen(stat_ret), 0);
    }

    // 结束信号
    /* puts("ls finish");//解决bug，这个比较稳定 */
    fflush(stdout);//ls的bug解决，clang编译无bug,gcc编译好像还会出现
    send(fd, "end", 4, 0);

    closedir(dirp);
    return 0;
}

int cmd_gets(int fd, char * cmd) {
    int ret = 0;
    train_t train_name;
    memset(&train_name, 0, sizeof(train_name));
    cycle_recv(fd, &train_name.length, sizeof(train_name.length));
    cycle_recv(fd, &train_name.buf, train_name.length);
    //printf("buf = %s\n",train_name.buf);
    ret = trans_file(fd, train_name.buf);
    if (-1 == ret) {
        char path[1 << 10] = {0};    
        train_t train_judge;
        memset(&train_judge, 0, sizeof(train_judge));
        train_judge.length = 1024;
        strcpy(train_judge.buf, path);
        ret = send(fd, &train_judge, sizeof(train_judge.length) + train_judge.length, 0);
    }
    return 0;
}

int cmd_rm(char * cmd) {
    int pos = 0;
    while (pos < strlen(cmd) && ' ' != cmd[pos])
        ++pos;
    while (pos < strlen(cmd) && ' ' == cmd[pos])
        ++pos;

    char buf[1 << 10] = {0};
    sprintf(buf, "%s %s", "rm -r", cmd + pos);
    system(buf);

    return 0;
}

int cmd_pwd(int fd, char * path) {
    char * wd = getcwd(NULL, 0);
    memset(path, 0, strlen(path));
    strcpy(path, wd);
    send(fd, path, strlen(path), 0);

    return 0;
}
