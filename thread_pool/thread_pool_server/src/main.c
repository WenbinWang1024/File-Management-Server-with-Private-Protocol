#include "../head/tcp_net_socket.h"
#include "../head/work_queue.h"
#include "../head/file_transfer.h"

int main(int argc, char ** argv) {
    // arg format: ./ftpserver ../conf/server.conf
    ARGS_CHECK(argc, 2);

    /* init zone */
    int ret = 0;
    // current path
    char * cur_path = getcwd(NULL, 0);
    ERROR_CHECK(cur_path, NULL, "getcwd");
    // tcp regist
    int sfd = tcp_regist(argv[1]);
    // thread settings
    const int MAX_THREAD_NO = 3;

    

    return 0;
}
