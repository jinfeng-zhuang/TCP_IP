#include <stdio.h>  
#include <winsock2.h>

#include <socket.h>

#define RECV_BUF_LIMIT    (1024)

int port = 10086;

unsigned char g_recv_buffer[RECV_BUF_LIMIT];

int main(int argc, char* argv[])
{
    SOCKET fd_server;
    SOCKET client_fd;
    int ret;

    if (-1 == network_start())
        goto server_socket_error;

    fd_server = socket_open();
    if (-1 == fd_server)
        goto server_socket_error;

    while (true)
    {
        client_fd = socket_listen(fd_server, port);
        if (-1 == client_fd)
            goto server_socket_error;

        printf("accept a client\n");

        while (true)
        {
            ret = socket_recv(client_fd, (unsigned char *)g_recv_buffer, RECV_BUF_LIMIT);
            if (-1 == ret) {
                break;
            }

            ret = socket_send(client_fd, (unsigned char *)g_recv_buffer, ret);
            if (-1 == ret)
                break;
        }
    }

server_socket_error:
    network_stop();

    return 0;
}
