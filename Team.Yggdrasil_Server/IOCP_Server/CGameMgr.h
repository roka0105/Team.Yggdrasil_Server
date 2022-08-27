#pragma once
class CSession;
struct t_GameInfo
{
	t_RoomInfo* m_roominfo;
	QuadNode* root;//ºΩ≈Õ
};
class CGameMgr
{
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


private:
	static CGameMgr* m_instance;
	list<t_GameInfo*> m_games;
};

