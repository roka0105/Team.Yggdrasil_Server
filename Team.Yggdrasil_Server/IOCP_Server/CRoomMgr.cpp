#include "pch.h"
#include "CRoomMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "CProtocolMgr.h"
#include "CLobbyMgr.h"
#include "CLoginMgr.h"
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
	for (int i = 0; i < 11; i++)
	{
		TCHAR roomname[BUFSIZE];
		ZeroMemory(roomname, BUFSIZE);
		_stprintf(roomname, _T("room %d"), i);
		t_RoomInfo* room = new t_RoomInfo(i, ENetObjectType::Room, roomname, const_cast<TCHAR*>(_T("1234")), nullptr);
		if (i == 10)
			m_max_page++;
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

	Packing(protocol, static_cast<int>(m_page_room_count), _session);
}

bool CRoomMgr::EnterRoomProcess(CSession* _session, CLobbyState::SendCompType& _statetype)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);

	if (subprotocol != (int)SUBPROTOCOL::NONE)
		return false;

	byte buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	_session->UnPacking(buf);
	//unpack 방 아이디 빼오기
	int roomindex = 0;
	TCHAR pw[STRINGSIZE];
	ZeroMemory(pw, STRINGSIZE);
	UnPacking(buf, roomindex, pw);
	//방입장 가능 여부 체크 
	ERRTYPE enter_result = ERRTYPE::NONE;
	t_RoomInfo* room;
	enter_result = EnterCheck(roomindex, &room, pw);
	if (enter_result == ERRTYPE::NONE)
	{
		_session->SetPlayer(room->sessions.size());
		_session->SetRoomID(roomindex);
		if (room->host == nullptr)
			room->host = _session;
		room->sessions.push_back(_session);
		_statetype = CLobbyState::SendCompType::EnterRoom;
	}

	//전송 프로토콜 room , enterroomresult
	protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::RoomResult));

	int curid = _session->GetPlayer()->GetID();
	for (CSession* session : room->sessions)
	{
		if (!memcmp(session, _session, sizeof(CSession)))
		{
			Packing(protocol, static_cast<int>(enter_result), room, session);
		}
		else
		{
			Packing(protocol, static_cast<int>(ERRTYPE::NONE_ANOTHER_ENTER), _session->GetPlayer(), session);
		}
	}

	if (enter_result == ERRTYPE::NONE)
		return true;
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

void CRoomMgr::RemoveSession(t_RoomInfo* _room, CSession* _session)
{
	bool is_host = false;
	for (auto session : _room->sessions)
	{
		if (memcmp(session, _session, sizeof(CSession)) == false)
		{
			if (memcmp(_room->host, session, sizeof(CSession)) == false)
				is_host = true;
			_room->sessions.remove(session);
			break;
		}
	}
	if (is_host)
	{
		if (_room->sessions.size() != 0)
			_room->host = _room->sessions.front();
		else
			_room->host = nullptr;
	}
}

void CRoomMgr::RoomProcess(CSession* _session, CRoomState::SendCompType& _statetype)
{
	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	unsigned long detailprotocol = CProtocolMgr::GetInst()->GetDetailProtocol(protocol);

	switch (static_cast<SUBPROTOCOL>(subprotocol))
	{
	case SUBPROTOCOL::Multi:
		switch (detailprotocol)
		{
		case static_cast<const unsigned long>(DETAILPROTOCOL::CharacterSelect):
			CharacterFunc(_session);
			break;
			case  static_cast<const unsigned long>(DETAILPROTOCOL::ReadySelect) | static_cast<const unsigned long>(DETAILPROTOCOL::HostReady) :
				//방장 레디
				HostReadyFunc(_session, _statetype);
				break;
				case  static_cast<const unsigned long>(DETAILPROTOCOL::ReadySelect) | static_cast<const unsigned long>(DETAILPROTOCOL::NomalReady) :
					//팀원 레디
					NomalReadyFunc(_session);
					break;
					case static_cast<const unsigned long>(DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(DETAILPROTOCOL::AllMsg) :
						ChattingFunc(_session);
						break;
					
		}
		break;
	case SUBPROTOCOL::Single:
		break;
	}


}

void CRoomMgr::CharacterFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	//받아온 정보 언팩하기
	_session->UnPacking(data);
	int roomid = -1;
	int type = 0;
	UnPacking(data, roomid, type);
	t_RoomInfo* room = FindRoom(roomid);
	ERRTYPE err_type = CharacterCheck(room, type, _session);

	if (err_type == ERRTYPE::NONE)
	{
		_session->GetPlayer()->SetType(static_cast<E_CharacterType>(type));
	}

	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::CharacterResult));
	int id = _session->GetPlayer()->GetID();
	for (CSession* session : room->sessions)
	{
		Packing(protocol, static_cast<int>(err_type), id, type, session);
	}
}

void CRoomMgr::NomalReadyFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	//받아온 정보 언팩하기
	_session->UnPacking(data);
	int roomid = -1;
	bool ready = false;
	UnPacking(data, roomid, ready);
	_session->GetPlayer()->SetReady(ready);
	t_RoomInfo* room = FindRoom(roomid);

	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::ReadyResult));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::NomalReady));

	int id = _session->GetPlayer()->GetID();
	for (CSession* session : room->sessions)
	{
		if (!memcmp(session, _session, sizeof(CSession)))
		{
			Packing(protocol, id, ready, false, _session);
		}
		else
		{
			Packing(protocol, id, ready, true, session);
		}

	}

	//모두 레디중인지 체크 모두 레디중이라면 호스트한테 시작버튼 누르라고 신호 보내기
	bool allready = AllReadyCheck(room);
	protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::ReadySelect));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::HostReady));
	Packing(protocol, allready, room->host);
}

void CRoomMgr::HostReadyFunc(CSession* _session, CRoomState::SendCompType& _statetype)
{
	CLockGuard<CLock> lock(m_lock);
	byte buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	_session->UnPacking(buf);
	int roomid = -1;
	int mapindex = -1;
	UnPacking(buf, roomid, mapindex);

	t_RoomInfo* room = FindRoom(roomid);
	bool allready = AllReadyCheck(room);//플레이어 세명 다 레디 상태일 때 게임 들어가기 체크 함수
	unsigned long protocol = 0;
	
	if (allready == true)
	{
		_statetype = CRoomState::SendCompType::EnterGame;

		CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::GAME));
		CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
		CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::ReadyResult));
		CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::HostReady));
		for (auto session : room->sessions)
		{
			Packing(protocol, allready, session);
		}
	}
	else
	{
		CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
		CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
		CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::ReadyResult));
		CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::HostReady));
		//게임시작 실패 방장한테만 보내기
		Packing(protocol, allready, _session);
	}
}

void CRoomMgr::ChattingFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	TCHAR msg[BUFSIZE];
	ZeroMemory(msg, BUFSIZE);
	TCHAR msg2[BUFSIZE];
	ZeroMemory(msg2, BUFSIZE);
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	bool result = false;
	int roomindex = 0;
	_session->UnPacking(data);
	UnPacking(data, roomindex, msg);

	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, (unsigned long)MAINPROTOCOL::ROOM);
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, (unsigned long)SUBPROTOCOL::Multi);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)DETAILPROTOCOL::ChatRecv);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)DETAILPROTOCOL::AllMsg);

	t_RoomInfo* room = FindRoom(roomindex);

	if (_tcslen(msg) != 0)
	{
		result = true;
		_stprintf(msg2, _T(" %s : %s"), _session->GetUserInfo()->id, msg);
		for (CSession* client : room->sessions)
		{
			Packing(protocol, result, msg2, client);
		}
		_tprintf(_T("%s\n"), msg2);
	}
	else
	{
		Packing(protocol, result, _session);
	}
}

void CRoomMgr::BackPageProcess(CSession* _session, CRoomState::SendCompType& _statetype)
{
	CLockGuard<CLock> lock(m_lock);

	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long detailprotocol = CProtocolMgr::GetInst()->GetDetailProtocol(protocol);
	if (detailprotocol != static_cast<unsigned long>(DETAILPROTOCOL::LobbyEnter))
		return;
	t_RoomInfo* room = FindRoom(_session->GetRoomID());
	RemoveSession(room,_session);
	protocol = 0;
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::LobbyResult));
	for (auto another : room->sessions)
	{
		Packing(protocol, _session->GetPlayer()->GetID(), another);
	}
	protocol = 0;
	//클라의 state를 변경하기 위해 로비 입장 결과부터 전송한다.
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::LOBBY));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::LobbyResult));
	_statetype = CRoomState::SendCompType::EnterLobby;
	Packing(protocol, true, _session);
	// 방 리스트 정보 보낸다.
    SendRoom(true, 0, _session);
	//로비에 들어와있는 유저 리스트에 유저정보 추가한다.
	CLobbyMgr::GetInst()->AddSession(_session);
}

void CRoomMgr::DisConnected(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);

	unsigned long protocol = 0;

	t_RoomInfo* room = FindRoom(_session->GetRoomID());
	RemoveSession(room, _session);
	protocol = 0;
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::ROOM));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::LobbyResult));
	for (auto another : room->sessions)
	{
		Packing(protocol, _session->GetPlayer()->GetID(), another);
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

void CRoomMgr::SendRoom(bool result, int page, CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned long protocol = 0;

	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::LOBBY));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::DETAILPROTOCOL::RoomlistResult));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::DETAILPROTOCOL::PageRoom));

	list<t_RoomInfo*>temp;

	for (t_RoomInfo* room : m_rooms[page])
	{
		temp.push_back(room);
	}

	Packing(protocol, result, page, temp, _session);
}

bool CRoomMgr::PageCheck(int page)
{
	CLockGuard<CLock> lock(m_lock);
	if (m_max_page < page || page < 0)
	{
		return false;
	}
	return true;
}

CRoomMgr::ERRTYPE CRoomMgr::EnterCheck(int _roomindex, t_RoomInfo** _roominfo, const TCHAR* _pw)
{
	CLockGuard<CLock> lock(m_lock);
	int page = 0;
	t_RoomInfo* room = FindRoom(_roomindex);
	if (room == nullptr)
	{
		return ERRTYPE::ERR_ROOMINDEX;
	}
	else if (room->sessions.size() < m_enter_limit)
	{
		if (!_tcscmp(room->password, _pw))
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
CRoomMgr::ERRTYPE CRoomMgr::CharacterCheck(const t_RoomInfo* _roominfo, int _type, CSession* _session)
{
	if (*(_session->GetPlayer()->GetReady()))
	{
		return ERRTYPE::ERR_CHARACTER;
	}
	for (CSession* session : _roominfo->sessions)
	{
		CPlayer* player = session->GetPlayer();

		const int* type = reinterpret_cast<const int*>(player->GetType());
		if (*type == _type)
		{
			if (*(player->GetReady()) == true)
			{
				return ERRTYPE::ERR_CHARACTER;
			}
		}
	}
	return ERRTYPE::NONE;
}
bool CRoomMgr::AllReadyCheck(t_RoomInfo* _room)
{
	int count = 0;
	for (auto session : _room->sessions)
	{
		CPlayer* player = session->GetPlayer();
		if (*(player->GetReady()) == true)
		{
			count++;
		}
	}
	if (count == m_enter_limit)
	{
		return true;
	}

	return false;
}
t_RoomInfo* CRoomMgr::FindRoom(int _roomindex)
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
			return room;
		}
	}
	return nullptr;
}


CRoomMgr::CRoomMgr()
{
	m_lock = new CLock();
}
CRoomMgr::~CRoomMgr()
{
	delete m_lock;
}

void CRoomMgr::Packing(unsigned long _protocol, bool _allready, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;

	memcpy(_buf, &_allready, sizeof(bool));
	size += sizeof(bool);
	ptr = _buf;
	_session->Packing(_protocol, ptr, size);
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
	if (result == static_cast<int>(ERRTYPE::NONE)) //입장 성공
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
		int hostid = host->GetPlayer()->GetID();
		memcpy(ptr, &hostid, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		//플레이어들 정보 (닉네임,선택한 캐릭터 정보)
		int objtype = static_cast<int>(ENetObjectType::Player);
		for (auto session : _room->sessions)
		{
			CPlayer* player = session->GetPlayer();
			//방에서의 아이디(순번)
			int playerid = player->GetID();
			memcpy(ptr, &playerid, sizeof(int));
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
		}
		int myid = _session->GetPlayer()->GetID();
		//세션의 아이디
		memcpy(ptr, &myid, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
#pragma endregion
	}

	ptr = buf;
	_session->Packing(_protocol, ptr, size);
}
void CRoomMgr::Packing(unsigned long _protocol, int _result, CPlayer* _enter_player, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	int strsize = 0;
	int objtype = static_cast<int>(ENetObjectType::Player);

	memcpy(ptr, &_result, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	int playerid = _enter_player->GetID();
	memcpy(ptr, &playerid, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	//오브젝트타입
	memcpy(ptr, &objtype, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	strsize = _tcslen(_enter_player->Getname()) * CODESIZE;
	memcpy(ptr, &strsize, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, _enter_player->Getname(), strsize);
	ptr += strsize;
	size += strsize;
	//캐릭터 타입
	memcpy(ptr, _enter_player->GetType(), sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	//레디상태 정보
	memcpy(ptr, _enter_player->GetReady(), sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);

	int myid = _session->GetPlayer()->GetID();
	//세션의 아이디
	memcpy(ptr, &myid, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	ptr = _buf;
	_session->Packing(_protocol, ptr, size);
}
void CRoomMgr::Packing(unsigned long _protocol, int _result, int _playerid, int _type, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	memcpy(ptr, &_result, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, &_playerid, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, &_type, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	ptr = _buf;
	_session->Packing(_protocol, ptr, size);
}

void CRoomMgr::Packing(unsigned long _protocol, int _playerid, bool _ready, bool _another, CSession* _session)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	memcpy(ptr, &_playerid, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, &_ready, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);
	memcpy(ptr, &_another, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);
	ptr = _buf;
	_session->Packing(_protocol, ptr, size);
}

void CRoomMgr::Packing(unsigned long _protocol, bool result, TCHAR* msg, CSession* _session)
{
	byte senddata[BUFSIZE];
	ZeroMemory(senddata, BUFSIZE);
	byte* ptr = senddata;
	int size = 0;
	int strsize = _tcslen(msg) * CODESIZE;
	memcpy(ptr, &result, sizeof(bool));
	size += sizeof(bool);
	ptr += sizeof(bool);
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, msg, strsize);
	size += strsize;

	_session->Packing(_protocol, senddata, size);
}

void CRoomMgr::UnPacking(byte* _recvdata, int& _roomindex, TCHAR* _pw)
{
	byte* ptr = _recvdata;
	int strsize = 0;
	memcpy(&_roomindex, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(_pw, ptr, strsize * CODESIZE);
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



void CRoomMgr::UnPacking(byte* _recvdata, int& _roomindex, int& _num)
{
	byte* ptr = _recvdata;

	memcpy(&_roomindex, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&_num, ptr, sizeof(int));
}

void CRoomMgr::UnPacking(byte* _recvdata, int& _roomindex, bool& _ready)
{
	byte* ptr = _recvdata;

	memcpy(&_roomindex, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&_ready, ptr, sizeof(bool));
}
