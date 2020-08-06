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
    char file_name[]="a.txt";
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
        cmd_gets(file_name,fd,cmd);
        break;
    case REMOVE:
        break;
    case PWD:
        cmd_pwd(fd, cmd);
        break;
    default:
        printf("Invalid command\n");
        break;
    }
    printf("client > ");

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

    char buf[1 << 10] = {0};
    while (1) {
        memset(buf, 0, sizeof(buf));
        recv(fd, buf, sizeof(buf), 0);
        if ('\0' == buf[0]) {
            break;
        }
        printf("%s\n", buf);
    }

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

int cmd_gets(char *file_name,int fd, char * cmd)
{
    train_t train;
    memset(&train, 0, sizeof(train));
    train.length = strlen(cmd);
    memcpy(train.buf, cmd, strlen(cmd));
    send(fd, &train, sizeof(train.length) + train.length, 0);
    //发送命令
    train_t trainname;
    memset(&trainname, 0, sizeof(trainname));
    trainname.length = strlen(file_name);
    memcpy(trainname.buf, file_name, strlen(file_name));
    send(fd, &trainname, sizeof(trainname.length) + trainname.length, 0);
    //发送传输文件名

    train_t trainnew;
    memset(&trainnew, 0, sizeof(trainnew));
    train_t train12;
    memset(&train12, 0, sizeof(train12));
    int ret=0;
    ret=cycle_recv(fd,&trainnew.length,sizeof(trainnew.length));
    ERROR_CHECK(ret,-1,"recvname");
    //printf("ret = %ld\n",trainnew.length);
    //得到文件名（其实用之前的也可以，但为了避免创建空余文件）
    ret = cycle_recv(fd,&trainnew.buf,trainnew.length);
    ERROR_CHECK(ret,-1,"recvname");
    //printf("ret = %s\n",trainnew.buf);

    int serverfd = open(trainnew.buf,O_RDWR|O_CREAT,0666);
    ERROR_CHECK(serverfd,-1,"open");

    //2.接收文件内容，写到文件中
    //每次接收数据时都要先接火车头
    //对于大文件，循环接收，循环写文件
    while(1)
    {
        //先接4个字节的控制信息
        cycle_recv(fd,&train12.length,sizeof(train12.length));
        ERROR_CHECK(ret,-1,"recvlen");
        //printf("datalen=%ld\n",train12.length);
        //接收到的dataLen等于0时，
        //代表文件传输结束，退出循环
        if(0 == train12.length){
            break;}
        ret = recv(fd,&train12.buf,train12.length,0);
        //printf("ret=%d\n",ret);
        //printf("buf = %s\n",train12.buf);
        ERROR_CHECK(ret,-1,"recv");
        ret=write(serverfd,train12.buf,train12.length);
        printf("writeret=%d\n",ret);
    }
    printf("success\n");
    close(serverfd);
    return 0;
}

