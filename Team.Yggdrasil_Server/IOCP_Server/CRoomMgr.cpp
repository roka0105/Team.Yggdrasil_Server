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

void CRoomMgr::Init()
{
	for (int i = 0; i < 5; i++)
	{
		TCHAR roomname[BUFSIZE];
		ZeroMemory(roomname, BUFSIZE);
		_stprintf(roomname, _T("room %d"), i);
		t_RoomInfo* room = new t_RoomInfo(i, roomname, const_cast<TCHAR*>(_T("1234")), nullptr);
		m_rooms[m_max_page].push_back(room);
	}

	
}

void CRoomMgr::End()
{
	
}


void CRoomMgr::AddRoom(CSession* _host)
{
	CLockGuard<CLock> lock(m_lock);

	TCHAR room_name[STRINGSIZE], room_pw[STRINGSIZE];
	ZeroMemory(room_name, STRINGSIZE);
	ZeroMemory(room_pw, STRINGSIZE);
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_host->UnPacking(data);
	//room �г���,�н����� unpack 
	UnPacking(data, room_name, room_pw);

	if (m_rooms[m_max_page].size() >= m_page_room_count)
	{
		m_max_page++;	
		m_rooms.insert({m_max_page,list<t_RoomInfo*>()});	
	}
	
	t_RoomInfo* room = new t_RoomInfo(m_rooms_count++, room_name,room_pw, _host);
	m_rooms[m_max_page].push_back(room);
}

void CRoomMgr::RemoveRoom(unsigned int _id)
{
	CLockGuard<CLock> lock(m_lock);
	for (int i = 0; i < m_max_page; i++)
	{
		for (t_RoomInfo* room : m_rooms[i])
		{
			if (room->id == _id)
			{
				m_rooms[i].remove(room);
			}
		}
	}
	
}

//void CRoomMgr::SendRoom(CSession* _session, unsigned int _id)
//{
//
//}

//void CRoomMgr::SendRoom(CSession* _session)
//{
//	CLockGuard<CLock> lock(m_lock);
//	unsigned long protocol = 0;
//	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, (unsigned long)MAINPROTOCOL::ROOM);
//	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, (unsigned long)CLobbyMgr::SUBPROTOCOL::Multi);
//	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)CLobbyMgr::DETAILPROTOCOL::RoomlistUpdate);
//	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)CLobbyMgr::DETAILPROTOCOL::AllRoom);
//
//	// �� ����Ʈ(���ȣ = ID, �� �̸�, �������, �� ���� �ο���, ���ο�����)
//	// structsize = uint + uint + str + uint + uint + uint = 20+32 = 52byte
//	// datasize = uint + structsize*n; = 4+ 52*n 
//	// packetsize= uint + uint + ulong + datasize ; = 12+datasize;
//	// ������ = 16
//	// ������ = 52*n
//	// ����ũ�� = 4096
//	// �ѹ��� ���� �� �ִ� �� ���� ������ 78.4615.... �ε� �����ϰ� 77 ������ ���.
//
//	int forcount = 0;
//	int remainder = 0;
//	int startindex = 0;
//	int endindex = m_rooms.size();
//	int count = 0;
//	if (m_rooms.size() > m_packet_room_count)
//	{
//		forcount = m_rooms.size() / m_packet_room_count;
//		remainder = m_rooms.size() % m_packet_room_count;
//		endindex = m_packet_room_count;
//	}
//	list<t_RoomInfo*> templist(m_rooms);
//	for (int i = 0; i < forcount + 1; i++)
//	{
//		list<t_RoomInfo*> roomlist;
//		
//
//		for (int j = startindex; j < endindex; j++)
//		{
//			if (count == endindex)
//			{
//				break;
//			}
//			roomlist.push_back(templist.front());
//			templist.pop_front();
//			count++;
//		}
//
//		if (i + 1 == forcount)
//		{
//			startindex = endindex;
//			endindex = endindex + remainder;
//		}
//		else
//		{
//			startindex = endindex;
//			endindex = endindex + endindex;
//		}
//		Packing(protocol,roomlist,_session);
//	}
//}

void CRoomMgr::SendRoom(unsigned int page, CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::SetRoomInfo));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::PageInfo));


	int count = 0;
	
	list<t_RoomInfo*>temp;
	for (t_RoomInfo* room : m_rooms[page])
	{
		temp.push_back(room);
	}
	
	Packing(protocol,page,temp, _session);
}

CRoomMgr::CRoomMgr()
{
	m_lock = new CLock();
}
CRoomMgr::~CRoomMgr()
{
	delete m_lock;
}

void CRoomMgr::Packing(unsigned long _protocol, t_RoomInfo* _room, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;


	_session->Packing(_protocol, _buf, size);
}

void CRoomMgr::Packing(unsigned long _protocol,int page, list<t_RoomInfo*> _rooms, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	int strsize = 0;
	int forsize = _rooms.size();
	
	memcpy(ptr, &page, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, &forsize, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
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
		memcpy(ptr, room->name, strsize);
		ptr += strsize;
		size += strsize;
		//�� ���
		strsize = _tcslen(room->password) * CODESIZE;
		memcpy(ptr, &strsize, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		memcpy(ptr, room->password, strsize);
		ptr += strsize;
		size += strsize;
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
	
	_session->Packing(_protocol, _buf, size);
}
void CRoomMgr::UnPacking(byte* _recvdata, TCHAR* _name,TCHAR* _pw)
{
	byte* ptr = _recvdata;
	int strsize = 0;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_name, ptr, strsize * CODESIZE);
	ptr += strsize * CODESIZE;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_pw, ptr, strsize*CODESIZE);
}
