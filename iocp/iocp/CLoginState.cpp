#include "pch.h"
#include "CLoginState.h"
#include "CSession.h"
#include "CLoginMgr.h"
#include "CProtocolMgr.h"
void CLoginState::Recv()
{
	unsigned long protocol;
	unsigned long mainprotocol;
	char buf[BUFSIZE]; ZeroMemory(buf, BUFSIZE);
	m_session->UnPacking(protocol);
	mainprotocol = CProtocolMgr::GetInst()->GetMainProtocol(protocol);
	//현재 받아온게 메뉴정보인지 로그인정보인지 회원가입정보인지 구분
	switch ((CLoginMgr::MAINPROTOCOL)mainprotocol)
	{
	case CLoginMgr::MAINPROTOCOL::LOGIN:
		CLoginMgr::GetInst()->LoginProc(m_session);
		break;
	case CLoginMgr::MAINPROTOCOL::JOIN:
		CLoginMgr::GetInst()->JoinProc(m_session);
		break;
	case CLoginMgr::MAINPROTOCOL::LOGOUT:
		CLoginMgr::GetInst()->LogOutProc(m_session);
		break;
	case CLoginMgr::MAINPROTOCOL::LOBBY:
		CLoginMgr::GetInst()->EnterLobbyProc(m_session);
		is_lobby = true;
		break;
	}
}

void CLoginState::Send()
{
	//로그인중인지 체크
	if (is_lobby)
		m_session->SetState(m_session->GetLobbyState());
}
