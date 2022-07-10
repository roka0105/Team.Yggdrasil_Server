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
	//���� �޾ƿ°� �޴��������� �α����������� ȸ�������������� ����
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
	//�α��������� üũ
	if (is_lobby)
		m_session->SetState(m_session->GetLobbyState());
}
