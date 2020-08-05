#include "../head/commands.h"
//参考 ：https://www.cnblogs.com/usingnamespace-caoliu/p/9351773.html
void file_perm(mode_t mode,char * ret) {//打印文件权限
    //判断usr权限
    /* printf("%c", mode&S_IRUSR?'r':'-');//S_IRUSR 00400 */
    /* printf("%c", mode&S_IWUSR?'w':'-'); */
    /* printf("%c", mode&S_IXUSR?'x':'-'); */
    ret[1] = mode & S_IRUSR ? 'r' : '-';
    ret[2] = mode & S_IWUSR ? 'w' : '-';
    ret[3] = mode & S_IXUSR ? 'x' : '-';

    //判断grp权限
    /* printf("%c", mode&S_IRGRP?'r':'-'); */
    /* printf("%c", mode&S_IWGRP?'w':'-'); */
    /* printf("%c", mode&S_IXGRP?'x':'-'); */
    ret[4] = mode & S_IRGRP ? 'r' : '-';
    ret[5] = mode & S_IWGRP ? 'w' : '-';
    ret[6] = mode & S_IXGRP ? 'x' : '-';

    //判断oth权限
    /* printf("%c", mode&S_IROTH?'r':'-'); */
    /* printf("%c", mode&S_IWOTH?'w':'-'); */
    /* printf("%c", mode&S_IXOTH?'x':'-'); */
    ret[7] = mode & S_IROTH ? 'r' : '-';
    ret[8] = mode & S_IWGRP ? 'w' : '-';
    ret[9] = mode & S_IXOTH ? 'x' : '-';
}

void file_type(mode_t mode, char * ret) {//打印文件类型
    if (S_ISREG(mode)) {
        /* printf("-");//普通文件 */
        ret[0] = '-';
    }  
    else if (S_ISDIR(mode)) {
        /* printf("d");//目录文件 */
        ret[0] = 'd';
    }
    else if (S_ISCHR(mode)) {
        /* printf("c");//字符设备文件 */
        ret[0] = 'c';
    }
    else if (S_ISBLK(mode)) {
        /* printf("b");//块设备文件 */
        ret[0] = 'b';
    }
    else if (S_ISFIFO(mode)) {
        /* printf("p");//管道文件 */
        ret[0] = 'p';
    }
    else if (S_ISLNK(mode)) {
        /* printf("l");//链接文件 */
        ret[0] = 'l';
    }
    else{
        /* printf("s");//socket文件 */
        ret[0] = 's';
    }
}

