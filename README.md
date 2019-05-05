# TCP/IP

## ����ָ��

* һ��������������쳣������ 0 ˵���쳣����ȷ������
* һ���������Դ��ڲ�����û��ǰ�ù�ϵ���߼���������ǰһ������δ�����ʱ�� return ��������� socket_uninit
* Ϊ�˱���ά��״̬������ʱ������� socket �ر�
* ���� API ��Ҫʹ���˸���ֱ�۵Ĳ���

## API

	extern int socket_init(void);

	extern int socket_uninit(int fd);

	extern int socket_listen(int fd, short port, int backlog);

	extern int socket_connect(int fd, const char *hostname, short port);

	extern int socket_send(int fd, unsigned char *buffer, unsigned int length);

	extern int socket_recv(int fd, unsigned char *buffer, unsigned int length);

	extern int hostname_to_ipv4(char *hostname, char *ipv4);

## ����

* ���������ر� socket ������
* �������� net_start, net_stop, socket_open, socket_close �Ƚӿ�
* ֧�ֶ��߳�
* socket_listen ȥ�� backlog ����
