# File Management Server with Private Protocol

## Phase I

Realize the Internet connection between the server and clients. Start the server and client by typing `./ftpserver ../conf/server.conf` and `./client [ip] [port]` on bash respectively.

In Phase I, the client provides more than six clauses similar to bash commands. The fundamental commands are listed as below:

1. `cd` - change directory
2. `ls` - list files and directories
3. `puts` - upload local files to the server
4. `gets` - download files from server to the local
5. `remove` - delete files on the server
6. `pwd` - print working directory
7. other commands such as `mkdir` and etc.

The client only responds to legal commands.
