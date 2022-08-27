#pragma once
class CSession;
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
};

