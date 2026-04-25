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
    char buffer[1024] = {0};

    // 1. tao socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 2. unlink old socket
    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    // 3. bind ip port
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // 4. listen
    listen(server_fd, 3);

    // 5. accept
    new_socket = accept(server_fd, NULL, NULL);

    // 6. send/receive
    read(new_socket, buffer, 1024);
    std::cout << "nhan msg: " << buffer << std::endl;

    const char* msg = "hello unix server";
    send(new_socket, msg, strlen(msg), 0);

    // 7. close
    close(new_socket);
    close(server_fd);

    // 8. unlink
    unlink(SOCKET_PATH);

    return 0;
}
