# File Management Server with Private Protocol

## Phase I

Realize the Internet connection between the server and clients. Start the server and client by typing `./ftpserver ../conf/server.conf` and `./client [ip] [port]` on bash respectively.

In Phase I, the client provides more than six clauses similar to bash commands. The fundamental commands are listed as below:

1. `cd` - change directory
2. `ls` - list files and directories
3. `puts` - upload local files to the server
4. `gets` - download files from server to client
5. `remove` - delete files on the server
6. `pwd` - print working directory
7. other commands such as `mkdir` and etc.

In this phase, clauses 1 to 6 will be correctly responded. User types a clause on client and presses enter to send it to the server. The command should be pretreated before being sent to the server. When receiving the command, server analyzes and then executes it if correct. The result of command execution will be sent to the client and output to the user.
