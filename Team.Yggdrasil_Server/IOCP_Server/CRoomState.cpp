#include "pch.h"
#include "CRoomState.h"
#include "CProtocolMgr.h"
#include "CRoomMgr.h"

void CRoomState::Init()
{
	m_sendcom_type = SendCompType::None;
}

void CRoomState::Recv()
{
	//mainprotocol 분리
	unsigned long protocol = 0;
	
	m_session->UnPacking(protocol);
	unsigned long mainprotocol = CProtocolMgr::GetInst()->GetMainProtocol(protocol);
	//switch문 mainprotocol로 분기
	switch ((MAINPROTOCOL)mainprotocol)
	{
	case MAINPROTOCOL::ROOM:
		CRoomMgr::GetInst()->RoomProcess(m_session,m_sendcom_type);
		break;
	case MAINPROTOCOL::LOBBY:
		CRoomMgr::GetInst()->BackPageProcess(m_session,m_sendcom_type);
		break;
	}
}

void CRoomState::Send()
{
	switch (m_sendcom_type)
	{
	case SendCompType::BackPage:
		break;
	case SendCompType::EnterGame:
	    m_session->SetState(m_session->GetGameState());
		break;  
	case SendCompType::EnterLobby:
		m_session->SetState(m_session->GetLobbyState());
		break;
	}
}
