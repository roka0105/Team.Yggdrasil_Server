#include "pch.h"
#include "CGameMgr.h"
#include "CMapMgr.h"
#include "CSectorMgr.h"
#include "CRoomMgr.h"
#include "CLockGuard.h"
#include "CLock.h"
#include "CProtocolMgr.h"
#include "HexTile.h"

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
	unsigned long detailprotocol = 0;
	byte buf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);

	_session->UnPacking(protocol);
	subprotocol= CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	detailprotocol = CProtocolMgr::GetInst()->GetDetailProtocol(protocol);
	switch (static_cast<SUBPROTOCOL>(subprotocol))
	{
	case SUBPROTOCOL::SECTOR:
		//방번호 unpack
		InitSector(_session);
		break;
	case SUBPROTOCOL::Object:
		switch (static_cast<DETAILPROTOCOL>(detailprotocol))
		{
		case DETAILPROTOCOL::Tile:
			InitTile(_session);
			break;
		case DETAILPROTOCOL::Player:
			break;
		case DETAILPROTOCOL::Boss:
			break;
		case DETAILPROTOCOL::Item:
			break;
		}
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
	//sector당 소유하고 있을 viewnode들 설정.
	CSectorMgr::GetInst()->SetViewNode(_session);
	//오브젝트들 위치정보 클라한테 받아온거 unpack 해서 필요한 섹터의 타일에 정보 넣기
	
	//sector들의 위치정보 보냄
	CSectorMgr::GetInst()->SendInit(game->m_id, _session, game->m_mapinfo);
}

void CGameMgr::InitTile(CSession* _session)
{
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	list<Vector3> senters;
	float radius=0;
	//unpacking
	UnPacking(data,senters,radius);
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	for (auto vec : senters)
	{
		HexTile* hex = new HexTile(vec, radius);
		//이 위치에 부합하는 sector 검색
		QuadNode* sector = CSectorMgr::GetInst()->SerchObjectNode(game,vec);
		//해당 sector에 addtile 하기.
		sector->AddObject(hex);
	}
}



void CGameMgr::TestFunc(CSession* _session)
{
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	CSectorMgr::GetInst()->TestSendViewTileProcess(_session, game);
}

void CGameMgr::UnPacking(byte* _recvbuf, UINT& _roomid)
{
	byte* ptr = _recvbuf;
	memcpy(&_roomid, ptr, sizeof(UINT));
}

void CGameMgr::UnPacking(byte* _recvbuf, list<Vector3>& _poslist,float& _radius)
{
	byte* ptr = _recvbuf;

	int count = 0;
	memcpy(&count, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&_radius, ptr, sizeof(float));
	ptr += sizeof(float);
	for (int i = 0; i < count; i++)
	{
		Vector3 vec;
		memcpy(&vec.x, ptr, sizeof(float));
		ptr += sizeof(float);
		memcpy(&vec.y, ptr, sizeof(float));
		ptr += sizeof(float);
		memcpy(&vec.z, ptr, sizeof(float));
		ptr += sizeof(float);
		_poslist.push_back(vec);
	}
}



