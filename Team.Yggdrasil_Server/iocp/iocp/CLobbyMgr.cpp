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
	unsigned long detailprotocol = CProtocolMgr::GetInst()->GetDetailProtocol(protocol);
	//switch subprotocol
	switch ((SUBPROTOCOL)subprotocol)
	{
	case SUBPROTOCOL::Multi:
	{

		switch (detailprotocol)
		{
		case (const unsigned long)DETAILPROTOCOL::LobbyEnter:
			LobbyEnterFunc(_session);
			break;
		case (const unsigned long)DETAILPROTOCOL::CreateRoom:
			break;
		case (const unsigned long)DETAILPROTOCOL::RoomlistUpdate | (const unsigned long)DETAILPROTOCOL::PageRoom:
			PageRoomFunc(_session);
			break;
		case (const unsigned long)DETAILPROTOCOL::RoomlistUpdate | (const unsigned long)DETAILPROTOCOL::AllRoom:
			AllRoomFunc(_session);
			break;
		case (const unsigned long)DETAILPROTOCOL::ChatSend | (const unsigned long)DETAILPROTOCOL::AllMsg:
			break;
		case (const unsigned long)DETAILPROTOCOL::ChatSend | (const unsigned long)DETAILPROTOCOL::NoticeMsg:
			break;
		}
		break;
	}
	case SUBPROTOCOL::Sigle:
		switch (detailprotocol)
		{
		case (const unsigned long)DETAILPROTOCOL::LobbyEnter:
			break;
		}

		break;

	}
}

void CLobbyMgr::EnterRoomProcess(CSession* _session)
{
	//������ ���� ���� üũ 
	//������ ���ɽ� state room���� ����.
	//���� �������� room , enterroomresult
}

void CLobbyMgr::BackPageProcess(CSession* _session)
{
	//state ->login ���� 
	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	switch ((SUBPROTOCOL)subprotocol)
	{
	case SUBPROTOCOL::BackPage:

		break;
	}
}

void CLobbyMgr::LobbyEnterFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	CRoomMgr::GetInst()->SendRoom(_session);

}

void CLobbyMgr::AllRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	CRoomMgr::GetInst()->SendRoom(_session);
}

void CLobbyMgr::PageRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	CRoomMgr::GetInst()->SendRoom(0, _session);
}


