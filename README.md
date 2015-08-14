 Communication with stream socket
==================================
**Author**: Shengkui Leng

**E-mail**: lengshengkui@outlook.com


Description
-----------
This project is a demo for how to use stream socket(SOCK_STREAM) to communicate
between client and server.  And the server supports multiple clients via
multi-threading.

NOTES: There is no message boundaries in "SOCK_STREAM" type socket.

* * *

Build
-----------
(1) Open a terminal.

(2) chdir to the source code directory.

(3) Run "make"


Run
-----------
(1) Start the server:

>    $ ./server

You can specify the port number with argument:

>    $ ./server -p 5555

Notes:
>    The default port number used by server is 5555.

>    Use '-h' option to get more detail usage information of the server.

(2) Start the client to send request to server:

>    $ ./client

You can specify "server ip" and "server port number" with arguments:

>    $ ./client -s 127.0.0.1 -p 5555

Notes:
>    The default server port number is 5555.

>    The default server ip is 127.0.0.1.

>    Use '-h' option to get more detail usage information of the client.

