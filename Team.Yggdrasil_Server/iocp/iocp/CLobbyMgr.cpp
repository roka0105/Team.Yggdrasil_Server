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
	CRoomMgr::Create();
}

void CLobbyMgr::Destroy()
{
	delete instance;
	CRoomMgr::Destroy();
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
	//switch subprotocol
	switch ((SUBPROTOCOL)subprotocol)
	{
	case SUBPROTOCOL::LobbyEnter:
	{
		unsigned long detailprotocol = CProtocolMgr::GetInst()->GetDetailProtocol(protocol);
		const unsigned long multi_pageroom = (unsigned long)DETAILPROTOCOL::Multi | (unsigned long)DETAILPROTOCOL::PageRoom;
		const unsigned long multi_allroom = (unsigned long)DETAILPROTOCOL::Multi | (unsigned long)DETAILPROTOCOL::AllRoom;
		const unsigned long single = (unsigned long)DETAILPROTOCOL::Sigle;
		switch (detailprotocol)
		{
		case multi_pageroom:
			MultiPageRoomFunc(_session);
			break;
		case multi_allroom:
			MultiAllRoomFunc(_session);
			break;
		case single:
			break;
		}

		break;
	}
	case SUBPROTOCOL::CreateRoom:
		break;
	case SUBPROTOCOL::ChatSend:
		break;
	case SUBPROTOCOL::RoomlistUpdate:
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
}

void CLobbyMgr::MultiAllRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	CRoomMgr::GetInst()->SendRoom(_session);
}

void CLobbyMgr::MultiPageRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	CRoomMgr::GetInst()->SendRoom(0, _session);
}


