#include "pch.h"
#include "CSocket.h"
bool CSocket::WSASEND()
{
	wsasend();
	return true;
}

bool CSocket::WSASEND(char* _buf, int _size)
{
	t_sendbuf* sendbuf = new t_sendbuf(_buf, _size);
	m_send_que.push(sendbuf);
	
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

	t_sendbuf* sendbuf = m_send_que.front();
	ZeroMemory(&s_overlap.overlapped, sizeof(OVERLAPPED));

	wsa_sendbuf.len = sendbuf->sendbytes - sendbuf->com_sendbytes;
	wsa_sendbuf.buf = sendbuf->sendbuf + sendbuf->com_sendbytes;

	retval = WSASend(m_sock, &wsa_sendbuf, 1, &sendbytes, flags, &s_overlap.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			err_display("WSASend()"); // 이 경우만 수동으로 큐에 넣어준다.
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
	wsa_recvbuf.buf = m_trecvbuf.recvbuf + m_trecvbuf.com_recvbytes;

	retval = WSARecv(m_sock, &wsa_recvbuf, 1, &recvbytes, &flags, &r_overlap.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			err_display("WSARecv()");
			return false;
		}
	}
	return true;
}

void CSocket::SendListPush(t_sendbuf* _tsendbuf)
{
	m_send_que.push(_tsendbuf);
}

void CSocket::SendListPop()
{
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
	t_sendbuf* send = m_send_que.front();
	send->com_sendbytes += _cb_t;
	if (send->com_sendbytes == send->sendbytes)
	{
		send->com_sendbytes = send->sendbytes = 0;
		ZeroMemory(send->sendbuf, BUFSIZE);
		m_send_que.pop();
		return SOC::SOC_TRUE;
	}
	else
	{
		return SOC::SOC_FALSE;
	}
}

void CSocket::Init()
{
	
}
