#include <stdio.h>
#include <string.h>

#include "socket.h"

#define RECV_BUF_LIMIT    (1024)

const char *hostname = "localhost";
const short port = 81;

const char *message = "Hello Server";
unsigned char g_recv_buffer[RECV_BUF_LIMIT];

int main(int argc, char* argv[])
{
    int fd;
    int ret;

    fd = socket_init();
    if (fd < 0)
        goto socket_error;

    while (true)
    {
        if (-1 == socket_connect(fd, hostname, port))
            goto socket_error;

        while (true)
        {
            if (-1 == socket_send(fd, (unsigned char *)message, strlen(message)))
                goto socket_error;

            memset(g_recv_buffer, 0, RECV_BUF_LIMIT);
            ret = socket_recv(fd, (unsigned char *)g_recv_buffer, RECV_BUF_LIMIT);
            if (-1 == ret)
                goto socket_error;
            else
                printf("receive %d bytes\n", ret);

            printf("%s\n", g_recv_buffer);
        }
    }

socket_error:
    socket_uninit(fd);
    return 0;
}
