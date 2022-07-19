#include "pch.h"
#include "CLoginState.h"
#include "CSession.h"
#include "CLoginMgr.h"
#include "CProtocolMgr.h"
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
		CLoginMgr::GetInst()->EnterLobbyProcess(m_session);
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
