# TCP/IP

## ����ָ��

* һ��������������쳣������ 0 ˵���쳣����ȷ������
* һ���������Դ��ڲ�����û��ǰ�ù�ϵ���߼���������ǰһ������δ�����ʱ�� return ��������� socket_uninit
* Ϊ�˱���ά��״̬������ʱ������� socket �ر�
* ���� API ��Ҫʹ���˸���ֱ�۵Ĳ���
* ����ô�䶯�Ĳ��������� API ���棬�� backlog ��ѡ��ʹ�þ�̬����
* ����������������ﲻ����װ��Ŀ��

## API

	extern int socket_listen(short port);

	extern int socket_connect(const char *hostname, short port);

	extern int socket_send(int fd, unsigned char *buffer, unsigned int length);

	extern int socket_recv(int fd, unsigned char *buffer, unsigned int length);

## ����

* ֧�ֶ��߳�
* ���� timeout ����
* ÿ�� API �ڵ������ļ���ʵ�֣�������ά��
