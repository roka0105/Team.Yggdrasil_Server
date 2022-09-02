#pragma once
class CSession;
class Item;
class Player;
class Boss;
#include "CRoomMgr.h"
#include "CMapMgr.h"
struct t_DataInfo
{
	~t_DataInfo()
	{
		delete m_boss;
		int count = m_items.size();
		for (int i = 0; i < count; i++)
		{
			Item* item = m_items.front();
			m_items.pop_front();
			delete item;
		}
	}
	vector<CPlayer*> m_players;
	Boss* m_boss;
	list<Item*> m_items;
};
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
		delete m_datainfo;
	}
	static int idcount;
	int m_id;

	t_RoomInfo* m_roominfo;
	t_MapInfo* m_mapinfo;
	map<UINT, QuadNode*> m_leaf_nodes;
	t_DataInfo* m_datainfo;

};

class CGameMgr
{
public:
	enum class SUBPROTOCOL
	{
		NONE,
		SECTOR,
		Object,
		MAX
	};
	enum class DETAILPROTOCOL
	{
		NONE,
		Tile,
		Player,
		Boss,
		Item,
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
	void InitTile(CSession* _session);
	void InitPlayer(CSession* _session);
	void InitBoss(CSession* _session);
	void InitItem(CSession* _session);

	void AddGameInfo(t_GameInfo* _game);
	void RemoveGameInfo(UINT _gameid);
	t_GameInfo* FindGameInfo(int _roomid);

	//
	void TestFunc(CSession* _session);
	//
	void Packing(unsigned long _protocol,CSession* _session);
	void Packing(unsigned long _protocol, list<Vector3>& _poslist, CSession* _session);
	void UnPacking(byte* _recvbuf, UINT& _roomid);
	void UnPacking(byte* _recvbuf, list<Vector3>& _poslist,float& _radius);
	void UnPacking(byte* _recvbuf, list<Vector3>& _poslist);
	void UnPacking(byte* _recvbuf, Vector3& _pos);
private:
	static CGameMgr* m_instance;
	CLock* m_lock;
	list<t_GameInfo*> m_games;
};

