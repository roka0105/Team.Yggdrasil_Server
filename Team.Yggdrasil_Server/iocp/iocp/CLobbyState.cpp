#include "pch.h"
#include "CLobbyState.h"
#include "CLobbyMgr.h"
#include "CState.h"
#include "CProtocolMgr.h"
void CLobbyState::Recv()
{
	//mainprotocol 분리
	unsigned long protocol = 0;
	m_session->UnPacking(protocol);
	unsigned long mainprotocol = CProtocolMgr::GetInst()->GetMainProtocol(protocol);
	//switch문 mainprotocol로 분기
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
	//로비(로비에서 해야할 일 : 방만들기,전체채팅,방목록)
	//게임 (게임 방 입장시 정보 전송 후 sned완료되면 state 변경)
	//로그인 (이 경우는 클라가 로비에서 나와서 다시 메뉴선택을 할 경우 state login 에서 처리해 주기 때문에 여기서 state를 변경해줌)
}

void CLobbyState::Send()
{
}
