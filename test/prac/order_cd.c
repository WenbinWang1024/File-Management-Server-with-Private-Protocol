#include "../head/commands"

// 客户端将命令传输给服务端，等待服务端传回消息，打印在屏幕上
int order_cd(char *path,int sfd,char *order)
{
    int len = strlen(order);
    send(sfd,&len,4,0);
    send(sfd,&len,strlen(order),0);
    char message[100] = {0};
    char mes_len = 0;
    revc(sfd,&mes_len,4,0);
    recv(sfd,messege,mes_len,0);
    printf("Path: %s\n",message);
    return 0;
}
