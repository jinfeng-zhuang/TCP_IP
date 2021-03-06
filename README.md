# TCP/IP

## 编码指导

* 一个程序难免出现异常，返回 0 说明异常被正确处理了
* 一个函数可以存在并发的没有前置关系的逻辑，不能在前一个条件未满足的时候 return ，见最初的 socket_uninit
* 为了避免维护状态机，及时将错误的 socket 关闭
* 这套 API 主要使用了更加直观的参数
* 不怎么变动的参数不放在 API 里面，如 backlog ，选择使用静态配置
* 尽量简化数量，否则达不到封装的目的

## API

	extern int socket_listen(short port);

	extern int socket_connect(const char *hostname, short port);

	extern int socket_send(int fd, unsigned char *buffer, unsigned int length);

	extern int socket_recv(int fd, unsigned char *buffer, unsigned int length);

## 后续

* 支持多线程
* 完善 timeout 功能
* 每个 API 在单独的文件中实现，更容易维护
