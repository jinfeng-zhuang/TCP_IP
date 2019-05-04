#ifndef SOCKET_H
#define SOCKET_H

extern int socket_init(void);
extern int socket_uninit(int fd);
extern int socket_connect(int fd, const char *hostname, short port);
extern int socket_send(int fd, unsigned char *buffer, unsigned int length);
extern int socket_recv(int fd, unsigned char *buffer, unsigned int length);

#endif