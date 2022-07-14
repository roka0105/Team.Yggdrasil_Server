#include "pch.h"
#include "CPacket.h"

#include "CCrypt.h"

// 사이즈를 뺴고 암호화를 하여 패킹
// 암호화하고 복호화하고

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
		CSocket::WSASEND(buf, size);
		return;
	}
	//데이터의 size도 넣어준다.
	memcpy(ptr, &_size, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	memcpy(ptr, _data, _size);
	ptr = buf;
	size += _size;

	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);

	/// 암호화 코드
	/// 1번째 파라미터 : 패킹된 데이터
	/// 2번째 파라미터 : 암호화할 버프
	/// 3번째 파라미터 : 데이터 사이즈
	/// - 암호화시 주의점
	/// 패킹된 데이터 앞에 있는 전체 데이터 사이즈는 제외해서 암호화한다.
	
	//CCrypt::Encrypt((BYTE*)buf + sizeof(int), (BYTE*)buf + sizeof(int), size - sizeof(int));

	// 부모클래스에서 하는 일을 명시적으로 써준다.
	CSocket::WSASEND(buf, size);
}
//받아왔을때 패킷 구조 packetno/protocol/data

//protocol size data 
void CPacket::UnPacking(unsigned long &_protocol, byte* _buf)
{
	//클라에서 packetno 붙여서 보내줬어야 했는데 일단 테스트를 위해 packetno가 없다는 가정하에 진행.
	const byte* ptr = m_trecvbuf.recvbuf + sizeof(int);
	
	// 복호화 코드
	// - 받아온 데이터를 복호화한다.
	// - 클라이언트에서 데이터 패킹시 암호화한 경우에 사용한다.
	// CCrypt::Encrypt((BYTE*)m_trecvbuf.recvbuf, (BYTE*)m_trecvbuf.recvbuf, m_trecvbuf.recvbytes);

	int size = 0;
	memcpy(&_protocol, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&size, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_buf, ptr, size);
}
void CPacket::UnPacking(unsigned long& _protocol)
{
	//클라에서 packetno 붙여서 보내줬어야 했는데 일단 테스트를 위해 packetno가 없다는 가정하에 진행.
	const byte* ptr = m_trecvbuf.recvbuf + sizeof(int);
	
	int size = 0;
	memcpy(&_protocol, ptr, sizeof(int));
}

void CPacket::UnPacking(byte* _data)
{
	//클라에서 packetno 붙여서 보내줬어야 했는데 일단 테스트를 위해 packetno가 없다는 가정하에 진행.
	const byte* ptr = m_trecvbuf.recvbuf + sizeof(int);
	
	int size = 0;
	ptr += sizeof(int);

	memcpy(&size, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_data, ptr, size);
}
