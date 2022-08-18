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
	for (int i = 0; i < 7; i++)
	{
		TCHAR roomname[BUFSIZE];
		ZeroMemory(roomname, BUFSIZE);
		_stprintf(roomname, _T("room %d"), i);
		t_RoomInfo* room = new t_RoomInfo(i, ENetObjectType::Room, roomname, const_cast<TCHAR*>(_T("1234")), nullptr);
		m_rooms[m_max_page].push_back(room);
		m_rooms_count++;
	}
}

void CRoomMgr::End()
{

}

void CRoomMgr::SendInit(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned long protocol = 0;

	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::INIT));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::SUBPROTOCOL::Init));

	Packing(protocol, m_page_room_count, _session);
}

bool CRoomMgr::EnterRoomProcess(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	byte buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	_session->UnPacking(buf);
	//unpack 방 아이디 빼오기
	int roomindex = 0;
	TCHAR pw[STRINGSIZE];
	ZeroMemory(pw, STRINGSIZE);
	UnPacking(buf, roomindex,pw);
	//방입장 가능 여부 체크 
	ERRTYPE enter_result = ERRTYPE::NONE;
	t_RoomInfo* room;
	enter_result = EnterCheck(roomindex,&room,pw);
	if (enter_result == ERRTYPE::NONE)
	{
		_session->SetPlayer();
		if (room->host == nullptr)
			room->host = _session;
		room->sessions.push_back(_session);
	}
	//전송 프로토콜 room , enterroomresult
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::RoomResult));
	Packing(protocol,static_cast<int>(enter_result),room, _session);
	return false;
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
	//room 닉네임,패스워드 unpack 
	UnPacking(data, room_name, room_pw);

	if (m_rooms[m_max_page].size() >= m_page_room_count)
	{
		m_max_page++;
		m_rooms.insert({ m_max_page,list<t_RoomInfo*>() });
	}

	t_RoomInfo* room = new t_RoomInfo(m_rooms_count++, ENetObjectType::Room, room_name, room_pw, _host);
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
//	// 방 리스트(방번호 = ID, 방 이름, 모드정보, 방 참여 인원수, 방인원제한)
//	// structsize = uint + uint + str + uint + uint + uint = 20+32 = 52byte
//	// datasize = uint + structsize*n; = 4+ 52*n 
//	// packetsize= uint + uint + ulong + datasize ; = 12+datasize;
//	// 고정값 = 16
//	// 가변값 = 52*n
//	// 버퍼크기 = 4096
//	// 한번에 보낼 수 있는 방 정보 갯수는 78.4615.... 인데 안전하게 77 정도로 잡기.
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

	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::LOBBY));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::DETAILPROTOCOL::RoomlistResult));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::DETAILPROTOCOL::PageRoom));
	int count = 0;
	bool result = PageCheck(page);
	list<t_RoomInfo*>temp;
	if (result)
	{
		for (t_RoomInfo* room : m_rooms[page])
		{
			temp.push_back(room);
		}
	}
	Packing(protocol, result, page, temp, _session);
}

bool CRoomMgr::PageCheck(unsigned int page)
{
	if (m_max_page < page)
	{
		return false;
	}
	return true;
}

CRoomMgr::ERRTYPE CRoomMgr::EnterCheck(int _roomindex,t_RoomInfo** _roominfo,const TCHAR* _pw)
{
	int page = 0;
	if (_roomindex >= m_page_room_count - 1)
	{
		page = _roomindex % (m_page_room_count - 1) != 0 ?
			_roomindex / (m_packet_room_count - 1) + 1 : _roomindex / (m_packet_room_count - 1);
	}

	for (auto room : m_rooms[page])
	{
		if (room->id == _roomindex)
		{
			if (room->sessions.size() < m_enter_limit)
			{
				if (!_tcscmp(room->password,_pw))
				{
					*_roominfo = room;
					return ERRTYPE::NONE;
				}
				else
				{
					return ERRTYPE::ERR_PW;
				}
			}
			else
			{
				return ERRTYPE::ERR_MAXENTER;
			}
		}
	}	
	return ERRTYPE::ERR_ROOMINDEX;
}

CRoomMgr::CRoomMgr()
{
	m_lock = new CLock();
}
CRoomMgr::~CRoomMgr()
{
	delete m_lock;
}

void CRoomMgr::Packing(unsigned long _protocol, int _roomcount, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;

	memcpy(_buf, &_roomcount, sizeof(int));
	size += sizeof(int);

	_session->Packing(_protocol, _buf, size);
}

void CRoomMgr::Packing(unsigned long _protocol, t_RoomInfo* _room, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;

	//구현 안해놨음.

	_session->Packing(_protocol, _buf, size);
}

void CRoomMgr::Packing(unsigned long _protocol, bool result, int page, list<t_RoomInfo*> _rooms, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	int strsize = 0;
	int forsize = _rooms.size();
	//c# 은 bool 이 4byte임.
	memcpy(ptr, &result, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);
	if (result)
	{
		memcpy(ptr, &page, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		memcpy(ptr, &forsize, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		for (t_RoomInfo* room : _rooms)
		{
			//방번호
			memcpy(ptr, &room->id, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			memcpy(ptr, &room->type, sizeof(ENetObjectType));
			ptr += sizeof(ENetObjectType);
			size += sizeof(ENetObjectType);
			//방이름
			strsize = _tcslen(room->name) * CODESIZE;
			memcpy(ptr, &strsize, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			memcpy(ptr, room->name, strsize);
			ptr += strsize;
			size += strsize;
			//방 비번
			/*strsize = _tcslen(room->password) * CODESIZE;
			memcpy(ptr, &strsize, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			memcpy(ptr, room->password, strsize);
			ptr += strsize;
			size += strsize;*/
			//방모드
			memcpy(ptr, &room->mode, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			int enter_count = room->sessions.size();
			//방인원
			memcpy(ptr, &enter_count, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			//제한인원
			memcpy(ptr, &m_enter_limit, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);

		}
	}
	_session->Packing(_protocol, _buf, size);
}
void CRoomMgr::Packing(unsigned long _protocol, int result, t_RoomInfo* _room, CSession* _session)
{
	byte buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	byte* ptr = buf;
	int size = 0;
	int strsize = 0;
	memcpy(ptr, &result, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	if (result==static_cast<int>(ERRTYPE::NONE)) //입장 성공
	{
#pragma region 방정보
		//방번호
		memcpy(ptr, &_room->id, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		//오브젝트 타입
		memcpy(ptr, &_room->type, sizeof(ENetObjectType));
		ptr += sizeof(ENetObjectType);
		size += sizeof(ENetObjectType);
		//방 이름
		strsize = _tcslen(_room->name) * CODESIZE;
		memcpy(ptr, &strsize, sizeof(int));
		size += sizeof(int);
		ptr += sizeof(int);
		memcpy(ptr, _room->name, strsize);
		size += strsize;
		ptr += strsize;
		//방모드
		memcpy(ptr, &_room->mode, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		int enter_count = _room->sessions.size();
		//방인원
		memcpy(ptr, &enter_count, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		//제한인원
		memcpy(ptr, &m_enter_limit, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		//방장정보
		CSession* host = _room->host;
		strsize = _tcslen(host->GetUserInfo()->nickname) * CODESIZE;
		memcpy(ptr, &strsize, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		memcpy(ptr, host->GetUserInfo()->nickname, strsize);
		size += strsize;
		ptr += strsize;
		//플레이어들 정보 (닉네임,선택한 캐릭터 정보)
		int i = 0;
		int objtype = static_cast<int>(ENetObjectType::Player);
		for (auto session : _room->sessions)
		{
			CPlayer* player = session->GetPlayer();
			//방에서의 아이디(순번)
			memcpy(ptr, &i, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			//오브젝트타입
			memcpy(ptr, &objtype, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);

			strsize = _tcslen(player->Getname()) * CODESIZE;
			memcpy(ptr, &strsize, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			memcpy(ptr, player->Getname(), strsize);
			ptr += strsize;
			size += strsize;
			//캐릭터 타입
			memcpy(ptr, player->GetType(), sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);
			//레디상태 정보
			memcpy(ptr, player->GetReady(), sizeof(bool));
			ptr += sizeof(bool);
			size += sizeof(bool);
			i++;
		}
#pragma endregion
	}
	ptr = buf;
	_session->Packing(_protocol, ptr, size);
}
void CRoomMgr::UnPacking(byte* _recvdata, int& _roomindex,TCHAR* _pw)
{
	byte* ptr = _recvdata;
	int strsize = 0;
	memcpy(&_roomindex,ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_pw, ptr, strsize*CODESIZE);
}
void CRoomMgr::UnPacking(byte* _recvdata, TCHAR* _name, TCHAR* _pw)
{
	byte* ptr = _recvdata;
	int strsize = 0;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_name, ptr, strsize * CODESIZE);
	ptr += strsize * CODESIZE;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_pw, ptr, strsize * CODESIZE);
}
