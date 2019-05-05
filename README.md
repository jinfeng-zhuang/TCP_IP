# TCP/IP

## 编码指导

* 一个程序难免出现异常，返回 0 说明异常被正确处理了
* 一个函数可以存在并发的没有前置关系的逻辑，不能在前一个条件未满足的时候 return ，见最初的 socket_uninit
* 为了避免维护状态机，及时将错误的 socket 关闭


## API

	extern int socket_init(void);

	extern int socket_uninit(int fd);

	extern int socket_listen(int fd, short port, int backlog);

	extern int socket_connect(int fd, const char *hostname, short port);

	extern int socket_send(int fd, unsigned char *buffer, unsigned int length);

	extern int socket_recv(int fd, unsigned char *buffer, unsigned int length);

	extern int hostname_to_ipv4(char *hostname, char *ipv4);

## 后续

* 支持多线程