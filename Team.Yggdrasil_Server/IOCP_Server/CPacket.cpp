#include "pch.h"
#include "CPacket.h"

#include "CCrypt.h"

// ����� ���� ��ȣȭ�� �Ͽ� ��ŷ
// ��ȣȭ�ϰ� ��ȣȭ�ϰ�

void CPacket::Packing(unsigned long _protocol, byte* _data, int _size)
{
	byte buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);

	byte* ptr = buf + sizeof(int); // protocol/data // data -> protocol
	int size = 0;

	memcpy(ptr, &m_s_packNo, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	m_s_packNo++;

	memcpy(ptr, &_protocol, sizeof(unsigned long));
	ptr += sizeof(unsigned long);
	size += sizeof(unsigned long);

	if (_data == nullptr)
	{
		ptr = buf;
		memcpy(ptr, &size, sizeof(int));
		size += sizeof(int);
		CSocket::WSASEND(buf, size);
		return;
	}
	//�������� size�� �־��ش�.
	memcpy(ptr, &_size, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	memcpy(ptr, _data, _size);
	ptr = buf;
	size += _size;

	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);

	/// ��ȣȭ �ڵ�
	/// 1��° �Ķ���� : ��ŷ�� ������
	/// 2��° �Ķ���� : ��ȣȭ�� ����
	/// 3��° �Ķ���� : ������ ������
	/// - ��ȣȭ�� ������
	/// ��ŷ�� ������ �տ� �ִ� ��ü ������ ������� �����ؼ� ��ȣȭ�Ѵ�.
	
	//CCrypt::Encrypt(reinterpret_cast<BYTE*>(buf + sizeof(int)), reinterpret_cast<BYTE*>(buf + sizeof(int)), size - sizeof(int));

	// �θ�Ŭ�������� �ϴ� ���� ��������� ���ش�.
	CSocket::WSASEND(buf, size);
}
//�޾ƿ����� ��Ŷ ���� packetno/protocol/data

//protocol size data 
void CPacket::UnPacking(unsigned long &_protocol, byte* _buf)
{
	
	const byte* ptr = m_trecvbuf.recvbuf + sizeof(int);
	
	// ��ȣȭ �ڵ�
	// - �޾ƿ� �����͸� ��ȣȭ�Ѵ�.
	// - Ŭ���̾�Ʈ���� ������ ��ŷ�� ��ȣȭ�� ��쿡 ����Ѵ�.
	//CCrypt::Encrypt(reinterpret_cast<BYTE*>(m_trecvbuf.recvbuf), reinterpret_cast<BYTE*>(m_trecvbuf.recvbuf), m_trecvbuf.recvbytes);

	int size = 0;
	memcpy(&_protocol, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&size, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_buf, ptr, size);
}
void CPacket::UnPacking(unsigned long& _protocol)
{
	const byte* ptr = m_trecvbuf.recvbuf + sizeof(int)/*packetno*/;
	
	int size = 0;
	memcpy(&_protocol, ptr, sizeof(int));
}

void CPacket::UnPacking(byte* _data)
{
	const byte* ptr = m_trecvbuf.recvbuf + sizeof(int);
	
	int size = 0;
	ptr += sizeof(int);

	memcpy(&size, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_data, ptr, size);
}
