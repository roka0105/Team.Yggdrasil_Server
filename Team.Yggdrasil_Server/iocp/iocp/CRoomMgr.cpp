#include "pch.h"
#include "CRoomMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "CProtocolMgr.h"
#include "CLobbyMgr.h"
CRoomMgr* CRoomMgr::instance = nullptr;

CRoomMgr* CRoomMgr::GetInst()
{
	return instance;
}

void CRoomMgr::Create()
{
	if (instance == nullptr)
		instance = new CRoomMgr();
}

void CRoomMgr::Destroy()
{
	delete instance;
}

void CRoomMgr::AddRoom(TCHAR* _name, CSession* _host, unsigned int _mode)
{
	CLockGuard<CLock> lock(m_lock);
	t_RoomInfo* room = new t_RoomInfo(m_rooms.size(), _name, _host, _mode);
	m_rooms.push_back(room);
	m_max_page = m_rooms.size() / m_page_room_count;
}

void CRoomMgr::RemoveRoom(unsigned int _id)
{
	CLockGuard<CLock> lock(m_lock);
	for (t_RoomInfo* room : m_rooms)
	{
		if (room->id == _id)
		{
			m_rooms.remove(room);
		}
	}
}

//void CRoomMgr::SendRoom(CSession* _session, unsigned int _id)
//{
//
//}

void CRoomMgr::SendRoom(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, (unsigned long)MAINPROTOCOL::LOBBY);
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, (unsigned long)CLobbyMgr::SUBPROTOCOL::RoomlistUpdate);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)CLobbyMgr::DETAILPROCOTOL::Multi);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)CLobbyMgr::DETAILPROCOTOL::AllRoom);

	// �� ����Ʈ(���ȣ = ID, �� �̸�, �������, �� ���� �ο���, ���ο�����)
	// structsize = uint + uint + str + uint + uint + uint = 20+32 = 52byte
	// datasize = uint + structsize*n; = 4+ 52*n 
	// packetsize= uint + uint + ulong + datasize ; = 12+datasize;
	// ������ = 16
	// ������ = 52*n
	// ����ũ�� = 4096
	// �ѹ��� ���� �� �ִ� �� ���� ������ 78.4615.... �ε� �����ϰ� 77 ������ ���.

	int forcount = 0;
	int remainder = 0;
	int startindex = 0;
	int endindex = m_rooms.size();
	int count = 0;
	if (m_rooms.size() > m_packet_room_count)
	{
		forcount = m_rooms.size() / m_packet_room_count;
		remainder = m_rooms.size() % m_packet_room_count;
		endindex = m_packet_room_count;
	}
	list<t_RoomInfo*> templist = m_rooms;
	for (int i = 0; i < forcount + 1; i++)
	{
		list<t_RoomInfo*> roomlist;
		if (i == forcount + 1)
		{
			startindex = endindex;
			endindex = endindex + remainder;
		}
		else
		{
			startindex = endindex;
			endindex = endindex + endindex;
		}

		for (int j = startindex; j < endindex; j++)
		{
			if (count == endindex)
			{
				break;
			}
			roomlist.push_back(templist.front());
			templist.pop_front();
			count++;
		}
		Packing(protocol,roomlist,_session);
	}
}

void CRoomMgr::SendRoom(unsigned int page, CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, (unsigned long)MAINPROTOCOL::LOBBY);
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, (unsigned long)CLobbyMgr::SUBPROTOCOL::RoomlistUpdate);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)CLobbyMgr::DETAILPROCOTOL::Multi);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)CLobbyMgr::DETAILPROCOTOL::PageRoom);

	int count = 0;
	int room_count = m_rooms.size();
	vector<t_RoomInfo*>tempvector;
	for (t_RoomInfo* room : m_rooms)
	{
		tempvector.push_back(room);
	}
	list<t_RoomInfo*> roomlist;
	for (int i = page; i < (page+1) * m_page_room_count; i++)
	{
		if (i > room_count)
		{
			break;
		}
		roomlist.push_back(tempvector[i]);
	}
	Packing(protocol, roomlist, _session);
}

CRoomMgr::CRoomMgr()
{

}
CRoomMgr::~CRoomMgr()
{
}

void CRoomMgr::Packing(unsigned long _protocol, t_RoomInfo* _room, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;


	_session->Packing(_protocol, _buf, size);
}

void CRoomMgr::Packing(unsigned long _protocol, list<t_RoomInfo*> _rooms, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf + sizeof(int);
	int size = 0;
	int strsize = 0;
	for (t_RoomInfo* room : _rooms)
	{
		//���ȣ
		memcpy(ptr, &room->id, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		//���̸�
		strsize = _tcslen(room->name) * CODESIZE;
		memcpy(ptr, &strsize, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		memcpy(ptr, room->name, strsize * CODESIZE);
		ptr += strsize * CODESIZE;
		size += strsize * CODESIZE;
		//����
		memcpy(ptr, &room->mode, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		int enter_count = room->sessions.size();
		//���ο�
		memcpy(ptr, &enter_count, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		//�����ο�
		memcpy(ptr, &m_enter_limit, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

	}
	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	_session->Packing(_protocol, _buf, size);
}
void CRoomMgr::UnPacking(TCHAR* _name, unsigned int _mode)
{
}
