#pragma once
#include "CPacket.h"
#include "CState.h"
#include "CLoginState.h"
#include "CLobbyState.h"
#include "CRoomState.h"
#include "CSector.h"
#include "CPlayer.h"
#include "CGameState.h"

class CLock;
struct t_UserInfo
{
	t_UserInfo()
	{
		ZeroMemory(id, STRINGSIZE);
		ZeroMemory(pw, STRINGSIZE);
		ZeroMemory(nickname, STRINGSIZE);
		is_login = false;
	}
	t_UserInfo(TCHAR* _id, TCHAR* _pw, TCHAR* _nickname)
	{
		ZeroMemory(id, STRINGSIZE);
		ZeroMemory(pw, STRINGSIZE);
		ZeroMemory(nickname, STRINGSIZE);
		_tcscpy(id, _id);
		_tcscpy(pw, _pw);
		_tcscpy(nickname, _nickname);
		is_login = false;
	}

	TCHAR id[STRINGSIZE];
	TCHAR pw[STRINGSIZE];
	TCHAR nickname[STRINGSIZE];
	bool is_login;
};

class CSession :public CPacket
{
public:
	CSession(SOCKET _sock) :CPacket(_sock)
	{
		m_loginstate = new CLoginState(this);
		m_lobbystate = new CLobbyState(this);
		m_roomstate = new CRoomState(this);
		m_gamestate = new CGameState(this);
		m_curstate = m_loginstate;
		m_userinfo = new t_UserInfo();
	}
	~CSession() 
	{
		delete m_loginstate;
		delete m_lobbystate;
		delete m_roomstate;
		delete m_userinfo;
	}
	void Init();
	void End();

	t_UserInfo* GetUserInfo()
	{
		return m_userinfo;
	}
	void SetUserInfo(t_UserInfo _tuserinfo)
	{
		memcpy(&m_userinfo, &_tuserinfo, sizeof(t_UserInfo));
	}
	void SetUserInfo(TCHAR* _id, TCHAR* _pw, TCHAR* _nick, bool _flag)
	{
		ZeroMemory(m_userinfo->id, STRINGSIZE);
		ZeroMemory(m_userinfo->pw, STRINGSIZE);
		ZeroMemory(m_userinfo->nickname, STRINGSIZE);
		if (_id == NULL || _pw == NULL || _nick == NULL)
		{
			m_userinfo->is_login = _flag;
			return;
		}
		_tcsncpy(m_userinfo->id, _id,_tcslen(_id)*CODESIZE);
		_tcscpy(m_userinfo->pw, _pw);
		_tcscpy(m_userinfo->nickname, _nick);
		m_userinfo->is_login = _flag;
	}
    void SetSector(QuadNode* _nodesector)
    {
        m_sector = _nodesector;
    }
	CSector* GetSector()
	{
		return m_sector;
	}
	CState* GetState() { return m_curstate; }
	CState* GetLoginState() 
    { 
        m_loginstate->Init();
        return m_loginstate; 
    }
	CState* GetLobbyState() 
	{
		m_lobbystate->Init();
		return m_lobbystate; 
	}
	CState* GetRoomState() 
	{ 
		m_roomstate->Init();
		return m_roomstate; 
	}
	CState* GetGameState()
	{
		m_gamestate->Init();
		return m_gamestate;
	}
	void SetState(CState* _state)
	{
		m_curstate = _state;
	}
	void SetPlayer(int _index) 
	{
		if (m_player == nullptr)
			m_player = new CPlayer(_index,m_userinfo->nickname, E_CharacterType::None, Vector3(0, 0, 0));
		else
			m_player->SetInfo(m_userinfo->nickname, E_CharacterType::None, Vector3(0, 0, 0));
	};
	int GetRoomID()
	{
		return m_roomid;
	}
	void SetRoomID(int _index)
	{
		m_roomid = _index;
	}
	
	CPlayer* GetPlayer() { return m_player; };
private:
	t_UserInfo* m_userinfo;
	// STATE
	CState* m_curstate;
	CLoginState* m_loginstate;
	CLobbyState* m_lobbystate;
	CRoomState* m_roomstate;
	CGameState* m_gamestate;
    QuadNode* m_sector;
	int m_roomid;
	CPlayer* m_player;
	friend class CState;
	//int substate;
};


