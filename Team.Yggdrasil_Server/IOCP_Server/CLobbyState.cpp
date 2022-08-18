#include "pch.h"
#include "CLobbyState.h"
#include "CLobbyMgr.h"
#include "CState.h"
#include "CProtocolMgr.h"
#include "CRoomMgr.h"
void CLobbyState::Init()
{
	m_sendcom_type = SendCompType::None;
}
void CLobbyState::Recv()
{
	//mainprotocol �и�
	unsigned long protocol = 0;
	m_session->UnPacking(protocol);
	unsigned long mainprotocol = CProtocolMgr::GetInst()->GetMainProtocol(protocol);
	//switch�� mainprotocol�� �б�
	switch ((MAINPROTOCOL)mainprotocol)
	{
	case MAINPROTOCOL::LOBBY:
		CLobbyMgr::GetInst()->LobbyProcess(m_session);
		break;
	case MAINPROTOCOL::LOGIN:
		CLobbyMgr::GetInst()->BackPageProcess(m_session);
		m_sendcom_type = SendCompType::BackPage;
		break;
	case MAINPROTOCOL::ROOM:
		bool result = CRoomMgr::GetInst()->EnterRoomProcess(m_session);
		if (result)
		{
			m_sendcom_type = SendCompType::EnterRoom;
		}
		break;
	}
	//�κ�(�κ񿡼� �ؾ��� �� : �游���,��üä��,����)
	//���� (���� �� ����� ���� ���� �� sned�Ϸ�Ǹ� state ����)
	//�α��� (�� ���� Ŭ�� �κ񿡼� ���ͼ� �ٽ� �޴������� �� ��� state login ���� ó���� �ֱ� ������ ���⼭ state�� ��������)
}

void CLobbyState::Send()
{
	switch (m_sendcom_type)
	{
	case SendCompType::BackPage:
		m_session->SetState(m_session->GetLoginState());
		break;
	case SendCompType::EnterRoom:
		m_session->SetState(m_session->GetRoomState());
		break;
	}
}
