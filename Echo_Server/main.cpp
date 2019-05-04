#include <stdio.h>  
#include <winsock2.h>  

#pragma comment(lib, "ws2_32.lib")

#define RECV_BUF_LIMIT    (1024)

int port = 80;
int backlog = 5;

unsigned char g_recv_buffer[RECV_BUF_LIMIT];

int main(int argc, char* argv[])
{
    // WSA
    WORD socket_version = MAKEWORD(2, 2);
    WSADATA wsa_data;

    // Server
    SOCKET fd_server;
    sockaddr_in sin;

    // Clients
    SOCKET client_fd;
    sockaddr_in client_addr;
    int client_addr_len;

    // Common
    int ret;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

    client_addr_len = sizeof(client_addr);

    if (WSAStartup(socket_version, &wsa_data) != 0)
        goto out;

    fd_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd_server == INVALID_SOCKET)
        goto wsa_clean;

    if (bind(fd_server, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
        goto server_socket_errror;

    if (listen(fd_server, backlog) == SOCKET_ERROR)
        goto server_socket_errror;

    while (true)
    {
        client_fd = accept(fd_server, (SOCKADDR *)&client_addr, &client_addr_len);
        if (client_fd == INVALID_SOCKET)
            goto server_socket_errror;

        while (true)
        {
            ret = recv(client_fd, (char *)g_recv_buffer, RECV_BUF_LIMIT, 0);
            if (SOCKET_ERROR == ret) {
                goto client_disconnect;
            }
            else {
                ret = send(client_fd, (char *)g_recv_buffer, ret, 0);
                if (SOCKET_ERROR == ret)
                    goto client_disconnect;
            }
        }

    client_disconnect:
        closesocket(client_fd);
    }

server_socket_errror:
    closesocket(fd_server);

wsa_clean:
    WSACleanup();

out:
    return 0;
}
