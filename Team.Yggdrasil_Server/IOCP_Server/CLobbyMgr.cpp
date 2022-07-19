#include "pch.h"
#include "CLobbyMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "CRoomMgr.h"
#include "CProtocolMgr.h"

CLobbyMgr* CLobbyMgr::instance = nullptr;
CLobbyMgr::CLobbyMgr()
{
	m_lock = new CLock();
}
CLobbyMgr::~CLobbyMgr()
{
	delete m_lock;
}

CLobbyMgr* CLobbyMgr::GetInst()
{
	return instance;
}

void CLobbyMgr::Create()
{
	if (instance == nullptr)
		instance = new CLobbyMgr();
}

void CLobbyMgr::Destroy()
{
	delete instance;
}

void CLobbyMgr::Init()
{
}

void CLobbyMgr::End()
{
}

void CLobbyMgr::LobbyProcess(CSession* _session)
{
	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	unsigned long detailprotocol = CProtocolMgr::GetInst()->GetDetailProtocol(protocol);
	//switch subprotocol
	switch (static_cast<SUBPROTOCOL>(subprotocol))
	{
	case SUBPROTOCOL::Multi:
	{

		switch (detailprotocol)
		{
		case static_cast<const unsigned long>(DETAILPROTOCOL::CreateRoom):
			CreateRoomFunc(_session);
			break;
		case static_cast<const unsigned long>(DETAILPROTOCOL::RoomlistUpdate) | static_cast<const unsigned long>(DETAILPROTOCOL::PageRoom):
			PageRoomFunc(_session);
			break;
		case static_cast<const unsigned long>(DETAILPROTOCOL::RoomlistUpdate) | static_cast<const unsigned long>(DETAILPROTOCOL::AllRoom):
			//AllRoomFunc(_session);
			break;
		case static_cast<const unsigned long>(DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(DETAILPROTOCOL::AllMsg):
			break;
		case static_cast<const unsigned long>(DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(DETAILPROTOCOL::NoticeMsg):
			break;
		}
		break;
	}
	case SUBPROTOCOL::Single:
		switch (detailprotocol)
		{
		
		}
		break;

	}
}

void CLobbyMgr::EnterRoomProcess(CSession* _session)
{
	//방입장 가능 여부 체크 
	//방입장 가능시 state room으로 변경.
	//전송 프로토콜 room , enterroomresult
}

void CLobbyMgr::BackPageProcess(CSession* _session)
{
	//state ->login 변경 
	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	switch (static_cast<SUBPROTOCOL>(subprotocol))
	{
	case SUBPROTOCOL::BackPage:

		break;
	}
}

//void CLobbyMgr::AllRoomFunc(CSession* _session)
//{
//	CLockGuard<CLock> lock(m_lock);
//	CRoomMgr::GetInst()->SendRoom(_session);
//}

void CLobbyMgr::CreateRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	TCHAR room_name[STRINGSIZE], room_pw[STRINGSIZE];
	ZeroMemory(room_name, STRINGSIZE);
	ZeroMemory(room_pw, STRINGSIZE);
	//room 닉네임,패스워드 unpack 

	//방 생성
	CRoomMgr::GetInst()->AddRoom(room_name, room_pw, _session);
}

void CLobbyMgr::PageRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned int page=0;
	//page unpack
	CRoomMgr::GetInst()->SendRoom(page, _session);
}


