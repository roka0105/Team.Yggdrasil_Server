#include "pch.h"
#include "CLoginState.h"
#include "CSession.h"
#include "CLoginMgr.h"
#include "CProtocolMgr.h"
#include "CLobbyMgr.h"
void CLoginState::Recv()
{
	unsigned long protocol;
	unsigned long mainprotocol;
	
	m_session->UnPacking(protocol);
	mainprotocol = CProtocolMgr::GetInst()->GetMainProtocol(protocol);
	//���� �޾ƿ°� �޴��������� �α����������� ȸ�������������� ����
	switch (static_cast<MAINPROTOCOL>(mainprotocol))
	{
	case MAINPROTOCOL::LOGIN:
		CLoginMgr::GetInst()->LoginProcess(m_session);
		break;
	case MAINPROTOCOL::LOBBY:
		is_lobby = true;
		CLoginMgr::GetInst()->EnterLobbyProcess(m_session);
		break;
	}
}

void CLoginState::Send()
{
	if (is_lobby)
	{
		m_session->SetState(m_session->GetLobbyState());
	}
}
