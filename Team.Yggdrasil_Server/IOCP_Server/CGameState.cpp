#include "pch.h"
#include "CGameState.h"
#include "CSession.h"
#include "CProtocolMgr.h"
#include "CGameMgr.h"
#include "CSectorMgr.h"
void CGameState::Recv()
{
	unsigned long protocol;
	unsigned long mainprotocol;

	m_session->UnPacking(protocol);
	mainprotocol = CProtocolMgr::GetInst()->GetMainProtocol(protocol);
	switch (static_cast<MAINPROTOCOL>(mainprotocol))
	{
	case MAINPROTOCOL::INIT:
		CGameMgr::GetInst()->LoadingProcess(m_session);
		break;
	case MAINPROTOCOL::TEST:
		CGameMgr::GetInst()->TestFunc(m_session);
		break;
	}
}
void CGameState::Send()
{

}
void CGameState::Init()
{
	m_sendcom_type = SendCompType::None;
}
