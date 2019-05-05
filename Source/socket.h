#ifndef SOCKET_H
#define SOCKET_H

#define IP_STRING_LENGTH   (16)

extern int network_start(void);

extern int network_stop(void);

extern int socket_open(void);

extern int socket_close(int fd);

extern int socket_listen(int fd, short port);

/*
 * hostname:
 *   "localhost"
 *   "127.0.0.1"
 *   "www.google.com"
 */
extern int socket_connect(int fd, const char *hostname, short port);

extern int socket_send(int fd, unsigned char *buffer, unsigned int length);

extern int socket_recv(int fd, unsigned char *buffer, unsigned int length);

/*
 * "192.168.100.100"
 */
extern int hostname_to_ipv4(const char *hostname, char *ipv4);

#endif