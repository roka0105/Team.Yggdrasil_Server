#include "pch.h"
#include "CGameMgr.h"
#include "CMapMgr.h"
#include "CSectorMgr.h"
#include "CRoomMgr.h"
#include "CLockGuard.h"
#include "CLock.h"
#include "CProtocolMgr.h"

CGameMgr* CGameMgr::m_instance = nullptr;
int t_GameInfo::idcount = 0;

void CGameMgr::Create()
{
	if (m_instance == nullptr)
	{
		m_instance = new CGameMgr();
	}
	CMapMgr::Create();
	CSectorMgr::Create();
}

void CGameMgr::Destroy()
{
	CMapMgr::Destroy();
	CSectorMgr::Destory();
	delete m_instance;
}

CGameMgr* CGameMgr::GetInst()
{
	return m_instance;
}

CGameMgr::CGameMgr()
{
	m_lock = new CLock();
}

CGameMgr::~CGameMgr()
{
	delete m_lock;
}

void CGameMgr::Init()
{
	CMapMgr::GetInst()->Init();
	CSectorMgr::GetInst()->Init();
}

void CGameMgr::End()
{
	CMapMgr::GetInst()->End();
	CSectorMgr::GetInst()->End();
}

void CGameMgr::GameProcess(CSession* _session)
{
	
}

void CGameMgr::LoadingProcess(CSession* _session)
{
	unsigned long protocol = 0;
	unsigned long subprotocol = 0;
	byte buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);

	_session->UnPacking(protocol);
	subprotocol= CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	switch (static_cast<SUBPROTOCOL>(subprotocol))
	{
	case SUBPROTOCOL::SECTOR:
		//¹æ¹øÈ£ unpack
		InitSector(_session);
		break;
	}
}

void CGameMgr::AddGameInfo(t_GameInfo* _game)
{
	CLockGuard<CLock> lock(m_lock);
	m_games.push_back(_game);
	CSectorMgr::GetInst()->AddQuadTree(_game,_game->m_mapinfo);
}

t_GameInfo* CGameMgr::FindGameInfo(int _roomid)
{
	for (auto game : m_games)
	{
		if (game->m_roominfo->id == _roomid)
		{
			return game;
		}
	}
	return nullptr;
}

void CGameMgr::InitSector(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	CSectorMgr::GetInst()->SetViewNode(_session);
	CSectorMgr::GetInst()->SendInit(game->m_id, _session, game->m_mapinfo);
}

void CGameMgr::TestFunc(CSession* _session)
{
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	CSectorMgr::GetInst()->TestSendViewSectorProcess(_session, game);
}

void CGameMgr::UnPacking(byte* _recvbuf, UINT& _roomid)
{
	byte* ptr = _recvbuf;
	memcpy(&_roomid, ptr, sizeof(UINT));
}



