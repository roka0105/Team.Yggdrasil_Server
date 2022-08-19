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
		CRoomMgr::GetInst()->RoomProcess(m_session);
		break;
	case MAINPROTOCOL::LOBBY:
		//CRoomMgr::GetInst()->BackPageProcess(m_session);
		m_sendcom_type = SendCompType::BackPage;
		break;
	}
}

void CRoomState::Send()
{
}
