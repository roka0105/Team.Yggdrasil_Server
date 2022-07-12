#pragma once
class CSession;

class CSessionMgr
{
public:
	void Init();
	void End();

	static CSessionMgr* GetInst();
	static void Create();
	static void Destroy();

private:
	static CSessionMgr* m_instance;

	CSessionMgr();
	~CSessionMgr();

public:
	CSession* AddSession(SOCKET _sock);
	void RemoveSession(CSession* _ptr);

private:
	list<CSession*> m_session_list;

};

