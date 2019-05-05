#include <stdio.h>  
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define IP_STR_LENGTH   (15)
#define TIMEOUT         (3000)

static int wsa_inited = false;
static int timeout = TIMEOUT;
static int listening = false;

int hostname_to_addr(char *hostname, struct sockaddr_in *addr)
{
    int ret;
    struct addrinfo *result, *cur;

    if ((!hostname) || (!addr))
        return -1;

    ret = getaddrinfo(hostname, NULL, NULL, &result);
    if (0 != ret)
        return -1;

    for (cur = result; cur != NULL; cur = cur->ai_next) {
        if (AF_INET == cur->ai_family) {
            memcpy(addr, cur->ai_addr, sizeof(struct sockaddr_in));

            printf("%s: %d.%d.%d.%d\n", hostname,
                (*addr).sin_addr.S_un.S_un_b.s_b1,
                (*addr).sin_addr.S_un.S_un_b.s_b2,
                (*addr).sin_addr.S_un.S_un_b.s_b3,
                (*addr).sin_addr.S_un.S_un_b.s_b4);

            break;
        }
    }

    return 0;
}

int hostname_to_ipv4(char *hostname, char *ip)
{
    int ret;
    struct addrinfo *result, *cur;
    struct sockaddr_in addr;

    if ((!hostname) || (!ip))
        return -1;

    ret = getaddrinfo(hostname, NULL, NULL, &result);
    if (0 != ret)
        return -1;

    for (cur = result; cur != NULL; cur = cur->ai_next) {
        if (AF_INET == cur->ai_family) {
            memcpy(&addr, cur->ai_addr, sizeof(struct sockaddr_in));
            sprintf_s(ip, IP_STR_LENGTH, "%d.%d.%d.%d\n",
                addr.sin_addr.S_un.S_un_b.s_b1,
                addr.sin_addr.S_un.S_un_b.s_b2,
                addr.sin_addr.S_un.S_un_b.s_b3,
                addr.sin_addr.S_un.S_un_b.s_b4);

            break;
        }
    }

    return 0;
}

int socket_init(void)
{
    WORD socket_version = MAKEWORD(2, 2);
    WSADATA wsa_data;

    int fd;

    if (false == wsa_inited) {
        if (WSAStartup(socket_version, &wsa_data) != 0)
            return -1;
        else
            wsa_inited = true;
    }

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd >= 0)
        return fd;

    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

    return -1;
}

int socket_uninit(int fd)
{
    if (false == wsa_inited)
        WSACleanup();

    return 0;
}

int socket_connect(int fd, const char *hostname, short port)
{
    sockaddr_in server_addr;

    int ret;

    if (false == wsa_inited)
        return -1;

    if (!hostname)
        return -1;

    ret = hostname_to_addr((char *)hostname, &server_addr);
    if (0 != ret)
        return -1;

    server_addr.sin_port = htons(port);

    ret = connect(fd, (SOCKADDR *)&server_addr, sizeof(struct sockaddr_in));
    if (-1 == ret) {
        closesocket(fd);
        return -1;
    }

    return 0;
}

int socket_send(int fd, unsigned char *buffer, unsigned int length)
{
    int ret;

    if (false == wsa_inited)
        return -1;

    if ((!buffer) || (!length))
        return -1;

    ret = send(fd, (const char *)buffer, length, 0);
    if (-1 == ret) {
        closesocket(fd);
        printf("send error, connection closed\n");
        return -1;
    }

    return ret;
}

int socket_recv(int fd, unsigned char *buffer, unsigned int length)
{
    int ret;

    if (false == wsa_inited)
        return -1;

    if ((!buffer) || (!length))
        return -1;

    ret = recv(fd, (char *)buffer, length, 0);
    if (-1 == ret) {
        closesocket(fd);
        printf("recv error, connection closed\n");
        return -1;
    }

    return ret;
}

void socket_error(void)
{
    int err;

    if (false == wsa_inited)
        return;

    err = WSAGetLastError();

    printf("socket error %d\n", err);
}

int socket_listen(int fd, short port, int backlog)
{
    sockaddr_in sin;
    int client_fd;
    sockaddr_in client_addr;
    int client_addr_len;

    if (false == wsa_inited)
        return -1;

    if (!listening) {
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.S_un.S_addr = INADDR_ANY;

        if (bind(fd, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
            printf("the port %d has been used, exit\n", port);
            goto socket_error;
        }

        if (listen(fd, backlog) == SOCKET_ERROR)
            goto socket_error;

        listening = true;
    }

    client_addr_len = sizeof(client_addr);

    client_fd = accept(fd, (SOCKADDR *)&client_addr, &client_addr_len);
    if (client_fd == INVALID_SOCKET)
        goto socket_error;

    return client_fd;

socket_error:
    closesocket(fd);
    return -1;
}
