#include "pch.h"
#include "CLobbyState.h"
#include "CLobbyMgr.h"
#include "CState.h"
#include "CProtocolMgr.h"
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
		break;
	case MAINPROTOCOL::ROOM:
		CLobbyMgr::GetInst()->EnterRoomProcess(m_session);
		break;
	}
	//�κ�(�κ񿡼� �ؾ��� �� : �游���,��üä��,����)
	//���� (���� �� ����� ���� ���� �� sned�Ϸ�Ǹ� state ����)
	//�α��� (�� ���� Ŭ�� �κ񿡼� ���ͼ� �ٽ� �޴������� �� ��� state login ���� ó���� �ֱ� ������ ���⼭ state�� ��������)
}

void CLobbyState::Send()
{
}
