#pragma once
#define BUFSIZE 4096

enum class IO_TYPE
{
	ACCEPT,
	SEND,
	RECV,
	DISCONNECT,
};

struct OVERLAP_EX
{
	OVERLAPPED overlapped;
	IO_TYPE type;
	void* session;
};

struct t_sendbuf
{
	t_sendbuf()
	{
		ZeroMemory(sendbuf, BUFSIZE);
		sendbytes = com_sendbytes = 0;
	}
	t_sendbuf(char* _data, int _sendbytes)
	{
		ZeroMemory(sendbuf, BUFSIZE);
		memcpy(sendbuf, _data, _sendbytes);
		sendbytes = _sendbytes;
		com_sendbytes = 0;
	}

	int com_sendbytes;
	int sendbytes;
	char sendbuf[BUFSIZE];
};

struct t_recvbuf
{
	t_recvbuf()
	{
		is_recvmode = false;
		recvbytes = sizeof(int);
		com_recvbytes = 0;
		ZeroMemory(recvbuf, BUFSIZE);
	}
	t_recvbuf(char* _data, int _recvbytes)
	{
		memcpy(recvbuf, _data, _recvbytes);
		recvbytes = _recvbytes;
		com_recvbytes = 0;
	}

	int com_recvbytes;
	int recvbytes;
	char recvbuf[BUFSIZE];
	bool is_recvmode;
};

class CSocket
{
public:
	CSocket(int _port)
	{
		int retval = 0;

		m_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (m_sock == INVALID_SOCKET)
		{
			err_quit("socket()");
		}
		ZeroMemory(&m_addr, sizeof(SOCKADDR_IN));
		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		m_addr.sin_port = htons(_port);

		retval = bind(m_sock, (SOCKADDR*)&m_addr, sizeof(m_addr));
		if (retval == SOCKET_ERROR)
		{
			err_quit("bind()");
		}
		retval = listen(m_sock, SOMAXCONN);
		if (retval == SOCKET_ERROR)
		{
			err_quit("listen()");
		}
	}
	CSocket(SOCKET _sock)
	{
		m_sock = _sock;
		int len = 0;
		SOCKADDR_IN addr;
		len = sizeof(addr);
		ZeroMemory(&m_addr, sizeof(SOCKADDR));
		getpeername(_sock, (SOCKADDR*)&m_addr, &len);
		printf("Á¢¼Ó %s %d\n", inet_ntoa(m_addr.sin_addr), ntohs(m_addr.sin_port));
	}
	CSocket() {}

	bool WSASEND(char* _buf, int _size);
	bool WSASEND();
	bool WSARECV();

	void SendListPush(t_sendbuf* _tsendbuf);
	void SendListPop();

	void CloseSocket() { closesocket(m_sock); }

	SOCKET Accept();

	SOC CompRecv(int _cb_t);
	SOC CompSend(int _cb_t);

	void DelayDataSend() 
	{
		if (m_send_que.size() != 0)
		{
			wsasend();
		}
	}

public:
	SOCKET GetSock() { return m_sock; }
	SOCKADDR_IN GetAddr() { return m_addr; }
	void SetSock(SOCKET _sock) { m_sock = _sock; }
	void SetAddr(SOCKADDR_IN _addr) { memcpy(&m_addr, &_addr, sizeof(SOCKADDR_IN)); }
	void Init();
	bool wsasend();



protected:
	OVERLAP_EX r_overlap;
	OVERLAP_EX s_overlap;
	SOCKET m_sock;
	SOCKADDR_IN m_addr;
	t_recvbuf m_trecvbuf;
	queue<t_sendbuf*> m_send_que;
};



