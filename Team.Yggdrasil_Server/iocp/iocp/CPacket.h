#pragma once
#include "CSocket.h"
class CPacket :public CSocket
{
public:
	CPacket() 
	{ 
		m_s_packNo = 1;
		m_r_packNo = 1;
	}
	CPacket(SOCKET _sock):CSocket(_sock)
	{
		m_s_packNo = 1;
		m_r_packNo = 1;
	}
	// ��ŷ, ����ŷ ����� ��������� 
	void Packing(unsigned long _protocol, char* _data, int _size);
	void UnPacking(unsigned long& _protocol, char* _buf);
	void UnPacking(unsigned long& _protocol);
	void UnPacking(char* _data);

private:
	int m_s_packNo; 
	int m_r_packNo;

};

