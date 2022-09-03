#pragma once
//#include "global.h"
//�� ��������� lock ������ ������� CLock ���漱���ѰŰ� cpp���� ���ŵ� �׷��� cpp���� ��Ŭ�����.
class CLock;


struct OVERLAP_EX
{
	OVERLAP_EX()
	{
		ZeroMemory(&overlapped,sizeof(OVERLAPPED));
		type = IO_TYPE::NONE;
		session = nullptr;
	}
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
	t_sendbuf(byte* _data, int _sendbytes)
	{
		ZeroMemory(sendbuf, BUFSIZE);
		memcpy(sendbuf, _data, _sendbytes);
		sendbytes = _sendbytes;
		com_sendbytes = 0;
	}

	int com_sendbytes;
	int sendbytes;
	byte sendbuf[BUFSIZE];
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
	t_recvbuf(byte* _data, int _recvbytes)
	{
		is_recvmode = false;
		memcpy(recvbuf, _data, _recvbytes);
		recvbytes = _recvbytes;
		com_recvbytes = 0;
	}

	int com_recvbytes;
	int recvbytes;
	byte recvbuf[BUFSIZE];
	bool is_recvmode;
};

class CSocket
{
public:
	CSocket();
	CSocket(int _port);
	CSocket(SOCKET _sock);
	~CSocket();

	bool WSASEND(byte* _buf, int _size);
	bool WSASEND();
	bool WSARECV();

	void SendListPush(t_sendbuf* _tsendbuf);
	void SendListPop();

	void CloseSocket() { closesocket(m_sock); }

	SOCKET Accept();

	SOC CompRecv(int _cb_t);
	SOC CompSend(int _cb_t);

	void DelayDataSend();
	

public:
	SOCKET GetSock() { return m_sock; }
	SOCKADDR_IN GetAddr() { return m_addr; }
	void SetSock(SOCKET _sock) { m_sock = _sock; }
	void SetAddr(SOCKADDR_IN _addr) { memcpy(&m_addr, &_addr, sizeof(SOCKADDR_IN)); }
	void Init();
	bool wsasend();



protected:
	CLock* m_lock;
	OVERLAP_EX r_overlap;
	OVERLAP_EX s_overlap;
	SOCKET m_sock;
	SOCKADDR_IN m_addr;
	t_recvbuf m_trecvbuf;
	queue<t_sendbuf*> m_send_que;
};



