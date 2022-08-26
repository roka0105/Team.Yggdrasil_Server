#include "pch.h"
#include "CSocket.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "CLogMgr.h"
CSocket::CSocket()
{
	m_lock = new CLock();
	m_sock = NULL;
	ZeroMemory(&m_addr, sizeof(m_addr));
	r_overlap = OVERLAP_EX();
	s_overlap = OVERLAP_EX();
}

CSocket::CSocket(int _port)
{
	m_lock = new CLock();

	int retval = 0;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		err_quit(_T("socket()"));
	}
	ZeroMemory(&m_addr, sizeof(SOCKADDR_IN));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_addr.sin_port = htons(_port);

	retval = bind(m_sock, reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr));
	if (retval == SOCKET_ERROR)
	{
		err_quit(_T("bind()"));
	}
	retval = listen(m_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit(_T("listen()"));
	}
}
CSocket::CSocket(SOCKET _sock)
{
	m_lock = new CLock();
	m_sock = _sock;
	int len = 0;
	SOCKADDR_IN addr;
	len = sizeof(addr);
	ZeroMemory(&m_addr, sizeof(SOCKADDR));
	getpeername(_sock, reinterpret_cast<SOCKADDR*>(&m_addr), &len);
	printf("접속 %s %d\n", inet_ntoa(m_addr.sin_addr), ntohs(m_addr.sin_port));
}
CSocket::~CSocket()
{
	delete m_lock;
}
bool CSocket::WSASEND()
{
	CLockGuard<CLock> lock(m_lock);
	wsasend();
	return true;
}

bool CSocket::WSASEND(byte* _buf, int _size)
{
	CLockGuard<CLock> lock(m_lock);
	t_sendbuf* sendbuf = new t_sendbuf(_buf, _size);
	m_send_que.push(sendbuf);
	CLogMgr::GetInst()->WriteLog(_T("push que size = %d\n"), m_send_que.size());
	if (m_send_que.size() == 1)
	{
		wsasend();
	}
	return true;
}

bool CSocket::wsasend()
{
	int retval = 0;
	DWORD sendbytes, flags;
	flags = 0;
	WSABUF wsa_sendbuf;
	CLogMgr::GetInst()->WriteLog(_T("send que size = %d\n"), m_send_que.size());
	t_sendbuf* sendbuf = m_send_que.front();
	ZeroMemory(&s_overlap.overlapped, sizeof(OVERLAPPED));

	wsa_sendbuf.len = sendbuf->sendbytes - sendbuf->com_sendbytes;
	wsa_sendbuf.buf = reinterpret_cast<char*>(sendbuf->sendbuf + sendbuf->com_sendbytes);

	retval = WSASend(m_sock, &wsa_sendbuf, 1, &sendbytes, flags, &s_overlap.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			err_display(_T("WSASend()")); // 이 경우만 수동으로 큐에 넣어준다.
			return false;
		}
	}
}

bool CSocket::WSARECV()
{
	int retval = 0;
	DWORD recvbytes, flags;
	flags = 0;
	ZeroMemory(&r_overlap.overlapped, sizeof(OVERLAPPED));
	WSABUF wsa_recvbuf;

	wsa_recvbuf.len = m_trecvbuf.recvbytes - m_trecvbuf.com_recvbytes;
	wsa_recvbuf.buf = reinterpret_cast<char*>(m_trecvbuf.recvbuf + m_trecvbuf.com_recvbytes);

	retval = WSARecv(m_sock, &wsa_recvbuf, 1, &recvbytes, &flags, &r_overlap.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			err_display(_T("WSARecv()"));
			return false;
		}
	}
	return true;
}

void CSocket::SendListPush(t_sendbuf* _tsendbuf)
{
	CLockGuard<CLock> lock(m_lock);
	m_send_que.push(_tsendbuf);
}

void CSocket::SendListPop()
{
	CLockGuard<CLock> lock(m_lock);
	m_send_que.pop();
}

SOCKET CSocket::Accept()
{
	SOCKADDR_IN _addr;
	int _addrlen = sizeof(_addr);
	ZeroMemory(&_addr, sizeof(SOCKADDR_IN));
	SOCKET sock = accept(m_sock, (SOCKADDR*)&_addr, &_addrlen);
	return sock;
}

SOC CSocket::CompRecv(int _cb_t)
{
	m_trecvbuf.com_recvbytes += _cb_t;
	if (!m_trecvbuf.is_recvmode)
	{
		if (m_trecvbuf.com_recvbytes == m_trecvbuf.recvbytes)
		{
			memcpy(&m_trecvbuf.recvbytes, m_trecvbuf.recvbuf, sizeof(int));
			ZeroMemory(m_trecvbuf.recvbuf, BUFSIZE);
			m_trecvbuf.com_recvbytes = 0;
			m_trecvbuf.is_recvmode = true;
			return SOC::SOC_FALSE;
		}
		else
		{
			return SOC::SOC_FALSE;
		}
	}
	else
	{
		if (m_trecvbuf.com_recvbytes == m_trecvbuf.recvbytes)
		{
			//CCrypt::Encrypt((BYTE*)m_trecvbuf.recvbuf, (BYTE*)m_trecvbuf.recvbuf, m_trecvbuf.recvbytes);
			m_trecvbuf.recvbytes = sizeof(int);
			m_trecvbuf.com_recvbytes = 0;
			m_trecvbuf.is_recvmode = false;
			return SOC::SOC_TRUE;
		}
		else
		{
			return SOC::SOC_FALSE;
		}
	}
}

SOC CSocket::CompSend(int _cb_t)
{
	CLockGuard<CLock>lock(m_lock);
	CLogMgr::GetInst()->WriteLog(_T("comp que size = %d\n"), m_send_que.size());
	t_sendbuf* send = m_send_que.front();
	send->com_sendbytes += _cb_t;
 	if (send->com_sendbytes == send->sendbytes)
	{
		send->com_sendbytes = send->sendbytes = 0;
		ZeroMemory(send->sendbuf, BUFSIZE);
		m_send_que.pop();
		delete send;
		return SOC::SOC_TRUE;
	}
	else
	{
		return SOC::SOC_FALSE;
	}
}

void CSocket::DelayDataSend()
{
	CLockGuard<CLock>lock(m_lock);
	if (m_send_que.size() != 0)
	{
		CLogMgr::GetInst()->WriteLog(_T("delay que size = %d\n"),m_send_que.size());
		wsasend();
	}
}

void CSocket::Init()
{
	
}
