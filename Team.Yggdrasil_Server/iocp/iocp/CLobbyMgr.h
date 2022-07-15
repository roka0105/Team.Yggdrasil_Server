#pragma once
#include "pch.h"
#include "CSession.h"

class CLock;
class CLobbyMgr
{
public :
	static CLobbyMgr* GetInst();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	void MultiModeProcess();
	void SingleModeProcess();
private:
	CLobbyMgr();
	~CLobbyMgr();
private :
	CLock* m_lock;
	static CLobbyMgr* instance;
	list<CSession*> lobby_session_list; //로비에 들어와 있는 유저 정보.
};

