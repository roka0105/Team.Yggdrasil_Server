#pragma once
#include "CSocket.h"
class CPacket :public CSocket
{
public:
	CPacket():CSocket()
	{ 
		m_s_packNo = 1;
		m_r_packNo = 1;
	}
	CPacket(SOCKET _sock):CSocket(_sock)
	{
		m_s_packNo = 1;
		m_r_packNo = 1;
	}
	// 패킹, 언패킹 기능을 명시적으로 
	void Packing(unsigned long _protocol, byte* _data, int _size);
	void UnPacking(unsigned long& _protocol, byte* _buf);
	void UnPacking(unsigned long& _protocol);
	void UnPacking(byte* _data);

private:
	int m_s_packNo; 
	int m_r_packNo;

};

