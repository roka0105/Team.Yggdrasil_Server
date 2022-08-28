#pragma once
class CSession;
#include "CRoomMgr.h"
#include "CMapMgr.h"

struct t_GameInfo
{
	t_GameInfo()
	{
		m_id = idcount++;
	}
	t_GameInfo(t_RoomInfo* _room, t_MapInfo* _map)
	{
		m_id = idcount++;
		m_roominfo = _room;
		m_mapinfo = _map;
	}
	~t_GameInfo()
	{
		//delete root;
	}
	static int idcount;
	int m_id;

	t_RoomInfo* m_roominfo;
	t_MapInfo* m_mapinfo;
	map<UINT, QuadNode*> m_leaf_nodes;
};

class CGameMgr
{
public:
	enum class SUBPROTOCOL
	{
		NONE,
		SECTOR,
		MAP,
		MAX
	};
	enum class DETAILPROTOCOL
	{
		NONE,
		
	};
public:
	static void Create();
	static void Destroy();
	static CGameMgr* GetInst();
private:
	CGameMgr();
	~CGameMgr();
public:
	void Init();
	void End();
	void GameProcess(CSession* _session);
	void LoadingProcess(CSession* _session);

	void InitSector(CSession* _session);

	void AddGameInfo(t_GameInfo* _game);
	t_GameInfo* FindGameInfo(int _roomid);

	//
	void TestFunc(CSession* _session);
	//
	void UnPacking(byte* _recvbuf, UINT& _roomid);
private:
	static CGameMgr* m_instance;
	CLock* m_lock;
	list<t_GameInfo*> m_games;
};

