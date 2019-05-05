#include <stdio.h>  
#include <winsock2.h>

#include <socket.h>

#pragma comment(lib, "ws2_32.lib")

#define RECV_BUF_LIMIT    (1024)

int port = 80;
int backlog = 5;

unsigned char g_recv_buffer[RECV_BUF_LIMIT];

int main(int argc, char* argv[])
{
    SOCKET fd_server;
    SOCKET client_fd;
    int ret;

    fd_server = socket_init();
    if (INVALID_SOCKET == fd_server)
        goto wsa_clean;

    while (true)
    {
        client_fd = socket_listen(fd_server, port, backlog);
        if (INVALID_SOCKET == client_fd)
            goto server_socket_errror;

        while (true)
        {
            ret = socket_recv(client_fd, (unsigned char *)g_recv_buffer, RECV_BUF_LIMIT);
            if (SOCKET_ERROR == ret) {
                goto client_disconnect;
            }
            else {
                ret = socket_send(client_fd, (unsigned char *)g_recv_buffer, ret);
                if (SOCKET_ERROR == ret)
                    goto client_disconnect;
            }
        }

    client_disconnect:
        socket_uninit(client_fd);
    }

server_socket_errror:
wsa_clean:
    socket_uninit(fd_server);

    return 0;
}
