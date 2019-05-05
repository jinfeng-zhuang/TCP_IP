#include <stdio.h>
#include <string.h>

#include "socket.h"

#define RECV_BUF_LIMIT    (1024)

const char *hostname = "localhost";
const short port = 10086;

const char *message = "Hello Server";
unsigned char g_recv_buffer[RECV_BUF_LIMIT];

char ip_str[IP_STRING_LENGTH];

int main(int argc, char* argv[])
{
    int fd;
    int ret;

    if (-1 == network_start())
        goto client_socket_error;

    if (-1 == hostname_to_ipv4(hostname, ip_str)) {
        goto client_socket_error;
    }

    fd = socket_open();
    if (fd < 0)
        goto client_socket_error;

    while (true)
    {
        printf("Connect to %s\n", ip_str);

        if (-1 == socket_connect(fd, hostname, port))
            goto client_socket_error;

        while (true)
        {
            if (-1 == socket_send(fd, (unsigned char *)message, strlen(message)))
                break;

            memset(g_recv_buffer, 0, RECV_BUF_LIMIT);

            ret = socket_recv(fd, (unsigned char *)g_recv_buffer, RECV_BUF_LIMIT);
            if (-1 == ret)
                break;

            printf("receive %d bytes\n", ret);
        }
    }

client_socket_error:
    network_stop();
    return 0;
}
