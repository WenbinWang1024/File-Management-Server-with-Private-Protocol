#include <func.h>

int cmd_cd(char *path,int sfd,char *cmd)
{
    int n=0;
    while(cmd[n]!=' ')
    {
        ++n;
    }
    ++n;  //移动到cd命令符后的路径字符串
    int ret=chdir(cmd+n);
    if(ret==-1)
    {
        char error[48]={"error:No such file or directory"};
        int error_len=strlen(error);
        send(sfd,&error_len,4,0);
        send(sfd,error,strlen(error),0);
    }
    else //若路径存在
    {
        char *p=getcwd(cmd+n,200);
        memset(path,0,strlen(path));
        strcpy(path,p); //将当前目录修改为cd命令符后的路径
        int path_len=strlen(path);
        send(sfd,&path_len,4,0);
        send(sfd,path,strlen(path),0);
    }
    return 0;
}
