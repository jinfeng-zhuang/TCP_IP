#include <stdio.h>  
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

static int socket_count = 0;

int get_server_addr(char *hostname, struct sockaddr_in *addr)
{
    int ret;
    struct addrinfo *result, *cur;

    if ((!hostname) || (!addr))
        return -1;

    ret = getaddrinfo(hostname, NULL, NULL, &result);
    if (0 != ret)
        return NULL;

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

// 两个并发的逻辑，有前置后置关系
int socket_init(void)
{
    WORD socket_version = MAKEWORD(2, 2);
    WSADATA wsa_data;

    int fd;

    if (0 == socket_count) {
        if (WSAStartup(socket_version, &wsa_data) != 0)
            return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd >= 0) {
        socket_count++;
        return fd;
    }

    return -1;
}

// 1,2是并列关系，没有前置后置关系，所以前面的出错不会影响后续的执行
// 推荐的编码思路
// 延伸：并发的编程思维
// 遇到错误就返回，会让并发的逻辑走不下去
int socket_uninit(int fd)
{
    if (0 == closesocket(fd)) // 1
        socket_count--;

    if (0 == socket_count) // 2
        WSACleanup();

    return 0;
}

int socket_connect(int fd, const char *hostname, short port)
{
    sockaddr_in server_addr;

    int ret;

    if (0 == socket_count)
        return -1;

    if (!hostname)
        return -1;

    ret = get_server_addr((char *)hostname, &server_addr);
    if (0 != ret)
        return -1;

    server_addr.sin_port = htons(port);

    ret = connect(fd, (SOCKADDR *)&server_addr, sizeof(struct sockaddr_in));
    if (-1 == ret)
        return -1;

    return 0;
}

int socket_send(int fd, unsigned char *buffer, unsigned int length)
{
    int ret;

    if (0 == socket_count)
        return -1;

    if ((!buffer) || (!length))
        return -1;

    ret = send(fd, (const char *)buffer, length, 0);

    return ret;
}

int socket_recv(int fd, unsigned char *buffer, unsigned int length)
{
    int ret;

    if (0 == socket_count)
        return -1;

    if ((!buffer) || (!length))
        return -1;

    ret = recv(fd, (char *)buffer, length, 0);

    return ret;
}

void socket_error(void)
{
    int err;

    err = WSAGetLastError();

    printf("socket error %d\n", err);
}
