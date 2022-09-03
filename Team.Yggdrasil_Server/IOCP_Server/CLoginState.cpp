#include "pch.h"
#include "CLoginState.h"
#include "CSession.h"
#include "CLoginMgr.h"
#include "CRoomMgr.h"
#include "CProtocolMgr.h"
#include "CLobbyMgr.h"
#include "CSectorMgr.h"

void CLoginState::Recv(t_ThreadInfo* _threadinfo)
{
	unsigned long protocol;
	unsigned long mainprotocol;
	
	m_session->UnPacking(protocol);
	_threadinfo->io_type = IO_TYPE::RECV;
	_threadinfo->cur_state = E_STATE::LOGIN;
	_threadinfo->protocol = protocol;
	mainprotocol = CProtocolMgr::GetInst()->GetMainProtocol(protocol);
	//현재 받아온게 메뉴정보인지 로그인정보인지 회원가입정보인지 구분
	switch (static_cast<MAINPROTOCOL>(mainprotocol))
	{
	case MAINPROTOCOL::LOGIN:
		CLoginMgr::GetInst()->LoginProcess(m_session);
		::CreateError();
		break;
	case MAINPROTOCOL::LOBBY:
		is_lobby = true;
		CLoginMgr::GetInst()->EnterLobbyProcess(m_session);
		break;
    case MAINPROTOCOL::INIT:
        CRoomMgr::GetInst()->SendInit(m_session);
        break;
	}
}

void CLoginState::Send(t_ThreadInfo* _threadinfo)
{
	_threadinfo->io_type = IO_TYPE::SEND;
	_threadinfo->cur_state = E_STATE::LOGIN;
	if (is_lobby)
	{
		m_session->SetState(m_session->GetLobbyState());
	}
}
