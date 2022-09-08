#pragma once
#include "pch.h"
#include "CSector.h"
#include "CGameMgr.h"

class CSector;
class GameObject;
class CSession;
class CSectorMgr
{
public:
	static CSectorMgr* GetInst();
	static void Create();
	static void Destory();
private:
	static CSectorMgr* m_instance;
	CSectorMgr();
	~CSectorMgr();

	void CreateQuadTree(t_GameInfo* _gameinfo, t_MapInfo* _mapinfo);
	void CreateChildren(QuadNode* _parent, Vector3 _senterpos, Vector3 _distance, int _curdepth, t_GameInfo* _gameinfo);
	
	QuadNode** SerchNode(QuadNode* _parent, int _id, int _curdepth, t_MapInfo* _mapinfo);
	void SetViewNode(QuadNode* _parent, int _curdepth, t_MapInfo* _mapinfo, UINT _gameid);
	QuadNode** SerchNode(QuadNode* _parent, Vector3 _pos, int _curdepth, t_MapInfo* _mapinfo);
	QuadNode** SerchObjectNode(QuadNode* _parent, Vector3 _pos, int _curdepth, t_MapInfo* _mapinfo);

public:
	void Init();
	void End();
	void AddQuadTree(t_GameInfo* _gameinfo,t_MapInfo* _mapinfo);
	void SendInit(UINT _gameid, CSession* _session, t_MapInfo* _mapinfo);
	QuadNode* SerchObjectNode(t_GameInfo* _gameinfo,Vector3 _pos);
	//test
	QuadNode* return_root(UINT gameid)
	{
		return m_roots[gameid];
	}
	
	// 미완성
	void SetViewNode(CSession* _session);
	void AddObjectNode(QuadNode* _parent, GameObject* obj, int _curdepth);
	void RemoveObjectNode(QuadNode* _parent, GameObject* obj, int _curdepth);
	
	
	bool IsInView(QuadNode* _parent, Vector3 _startview, Vector3 _endview);
	//플레이어 샌드패킷
	void PlayerSendPacket(CSession* _session, unsigned long _protocol, bool moveflag);
	//void checksector()
	//몬스터 샌드패킷
	//void MonsterSendPacket(CSession*,CMonster*, Protocol,moveflag)
	//아이템 샌드패킷
	//void ItemSendPacket(CSession*,Protocol,_onoff_flag)

	// TEST Func
	void TestSendViewSectorProcess(CSession* _session,t_GameInfo* _gameinfo,Vector3 _objpos);
	void TestSendViewTileProcess(CSession* _session, t_GameInfo* _gameinfo,Vector3 _objpos);
	void TestPlayerMove(CSession* _session, t_GameInfo* _gameinfo);
public:
	void Packing(unsigned long _protocol, Vector3 _startpos, Vector3 _endpos, float _h_distance, float _v_distance, int _sectorcount, CSession* _session);
	void Packing(unsigned long _protocol, list<Vector3>& _starts, Vector3& _distances, CSession* _session);
	void UnPacking(byte* _recvbuf, Vector3& _pos);
private:
	CLock* m_lock;
	map<UINT, QuadNode*> m_roots; // key = gameinfo id,  value = rootnode 
};

