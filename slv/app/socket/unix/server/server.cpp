#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char* SOCKET_PATH = "/tmp/unix_socket.sock";

int main()
{
    int server_fd;
    int new_socket;
    struct sockaddr_un address;
    char buffer[100] = {0};

    // 1. tao socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 2. unlink old socket
    unlink(SOCKET_PATH);

    // memset(&address, 0, sizeof(address));

    // 3. 

    return 0;
}
