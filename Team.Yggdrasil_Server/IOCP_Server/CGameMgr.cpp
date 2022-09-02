#include "pch.h"
#include "CGameMgr.h"
#include "CMapMgr.h"
#include "CSectorMgr.h"
#include "CRoomMgr.h"
#include "CLockGuard.h"
#include "CLock.h"
#include "CProtocolMgr.h"
#include "HexTile.h"
#include "Boss.h"
#include "Item.h"

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
	subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
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
			InitPlayer(_session);
			break;
		case DETAILPROTOCOL::Boss:
			InitBoss(_session);
			break;
		case DETAILPROTOCOL::Item:
			InitItem(_session);
			break;
		}
		break;
	}
}

void CGameMgr::AddGameInfo(t_GameInfo* _game)
{
	CLockGuard<CLock> lock(m_lock);
	_game->m_datainfo = new t_DataInfo();
	m_games.push_back(_game);
	CSectorMgr::GetInst()->AddQuadTree(_game, _game->m_mapinfo);
}

void CGameMgr::RemoveGameInfo(UINT _gameid)
{
	CLockGuard<CLock> lock(m_lock);

	for (auto game : m_games)
	{
		if (game->m_id == _gameid)
		{
			delete game;
			break;
		}
	}
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
	

	//sector들의 위치정보 보냄
	CSectorMgr::GetInst()->SendInit(game->m_id, _session, game->m_mapinfo);
}

void CGameMgr::InitTile(CSession* _session)
{
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	list<Vector3> senters;
	float radius = 0;
	//unpacking
	UnPacking(data, senters, radius);
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	for (auto vec : senters)
	{
		HexTile* hex = new HexTile(vec, radius);
		//이 위치에 부합하는 sector 검색
		QuadNode* sector = CSectorMgr::GetInst()->SerchObjectNode(game, vec);
		//해당 sector에 addtile 하기.
		sector->AddObject(hex);
	}
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::INIT));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Object));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::Tile));
	Packing(protocol,_session);
}

void CGameMgr::InitPlayer(CSession* _session)
{
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	list<Vector3> senters;
	float radius = 0;
	//unpacking
	UnPacking(data, senters);
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	int index = 0;
	for (auto vec : senters)
	{
		CSession* session = game->m_roominfo->sessions[index];
		CPlayer* player = session->GetPlayer();
		player->SetVector(vec);
		game->m_datainfo->m_players.push_back(player);

		//이 위치에 부합하는 sector 검색
		QuadNode* sector = CSectorMgr::GetInst()->SerchObjectNode(game, vec);
		session->SetSector(sector);
		//해당 sector에 addtile 하기.
		sector->AddObject(player);
		
		index++;
	}
	unsigned long protocol = 0;
	//test로 받아온 좌표로 스폰시키기.
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::INIT));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Object));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::Player));
	Packing(protocol, senters, _session);
}

void CGameMgr::InitBoss(CSession* _session)
{
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	Vector3 pos;
	float radius = 0;
	//unpacking
	UnPacking(data, pos);
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());

	Boss* boss = new Boss();
	boss->SetVector(pos);
	game->m_datainfo->m_boss = boss;

	//이 위치에 부합하는 sector 검색
	QuadNode* sector = CSectorMgr::GetInst()->SerchObjectNode(game, pos);
	//해당 sector에 addtile 하기.
	sector->AddObject(boss);
	//test로 받아온 좌표로 스폰시키기.
	unsigned long protocol = 0;
	list<Vector3> senters;
	senters.push_back(pos);
	//test로 받아온 좌표로 스폰시키기.
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::INIT));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Object));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::Boss));
	Packing(protocol, senters, _session);
}

void CGameMgr::InitItem(CSession* _session)
{
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	list<Vector3> senters;
	float radius = 0;
	//unpacking
	UnPacking(data, senters, radius);
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	for (auto vec : senters)
	{
		Item* item = new Item();
		item->SetVector(vec);
		game->m_datainfo->m_items.push_back(item);
		//이 위치에 부합하는 sector 검색
		QuadNode* sector = CSectorMgr::GetInst()->SerchObjectNode(game, vec);
		//해당 sector에 addtile 하기.
		sector->AddObject(item);
	}
}



void CGameMgr::TestFunc(CSession* _session)
{
	t_GameInfo* game = FindGameInfo(_session->GetRoomID());
	CSectorMgr::GetInst()->TestSendViewTileProcess(_session, game);
}

void CGameMgr::Packing(unsigned long _protocol, CSession* _session)
{
	_session->Packing(_protocol,nullptr,0);
}

void CGameMgr::Packing(unsigned long _protocol, list<Vector3>& _poslist, CSession* _session)
{
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	byte* ptr = data;
	int size = 0;
	int count = _poslist.size();
	memcpy(ptr, &count, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	for (auto pos : _poslist)
	{
		memcpy(ptr, &pos.x, sizeof(float));
		ptr += sizeof(float);
		size += sizeof(float);
		memcpy(ptr, &pos.y, sizeof(float));
		ptr += sizeof(float);
		size += sizeof(float);
		memcpy(ptr, &pos.z, sizeof(float));
		ptr += sizeof(float);
		size += sizeof(float);
	}
	ptr = data;
	_session->Packing(_protocol, ptr, size);
}

void CGameMgr::UnPacking(byte* _recvbuf, UINT& _roomid)
{
	byte* ptr = _recvbuf;
	memcpy(&_roomid, ptr, sizeof(UINT));
}

void CGameMgr::UnPacking(byte* _recvbuf, list<Vector3>& _poslist, float& _radius)
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

void CGameMgr::UnPacking(byte* _recvbuf, list<Vector3>& _poslist)
{
	byte* ptr = _recvbuf;

	int count = 0;
	memcpy(&count, ptr, sizeof(int));
	ptr += sizeof(int);

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

void CGameMgr::UnPacking(byte* _recvbuf, Vector3& _pos)
{
	byte* ptr = _recvbuf;

	Vector3 vec;
	memcpy(&vec.x, ptr, sizeof(float));
	ptr += sizeof(float);
	memcpy(&vec.y, ptr, sizeof(float));
	ptr += sizeof(float);
	memcpy(&vec.z, ptr, sizeof(float));
	ptr += sizeof(float);

}



