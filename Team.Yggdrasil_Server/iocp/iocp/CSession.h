#pragma once
#include "CPacket.h"
#include "CState.h"
#include "CLoginState.h"
#include "CLobbyState.h"
#define IDSIZE 32
#define PWSIZE 32
#define NAMESIZE 32

struct t_UserInfo
{
	t_UserInfo()
	{
		ZeroMemory(id, IDSIZE);
		ZeroMemory(pw, PWSIZE);
		ZeroMemory(nickname, NAMESIZE);
		is_login = false;
	}
	t_UserInfo(TCHAR* _id, TCHAR* _pw, TCHAR* _nickname)
	{
		ZeroMemory(id, IDSIZE);
		ZeroMemory(pw, PWSIZE);
		ZeroMemory(nickname, NAMESIZE);
		_tcscpy(id, _id);
		_tcscpy(pw, _pw);
		_tcscpy(nickname, _nickname);
		is_login = false;
	}

	TCHAR id[IDSIZE];
	TCHAR pw[PWSIZE];
	TCHAR nickname[NAMESIZE];
	bool is_login;
};

class CSession :public CPacket
{
public:
	CSession(SOCKET _sock) :CPacket(_sock)
	{
		m_loginstate = new CLoginState(this);
		m_lobbystate = new CLobbyState(this);
		m_curstate = m_loginstate;
		m_userinfo = new t_UserInfo();
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
		ZeroMemory(m_userinfo->id, IDSIZE);
		ZeroMemory(m_userinfo->pw, PWSIZE);
		ZeroMemory(m_userinfo->nickname, NAMESIZE);
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

	CState* GetState() { return m_curstate; }
	CState* GetLoginState() { return m_loginstate; }
	CState* GetLobbyState() { return m_lobbystate; }

	void SetState(CState* _state)
	{
		m_curstate = _state;
	}
private:
	t_UserInfo* m_userinfo;

	// STATE
	CState* m_curstate;
	CLoginState* m_loginstate;
	CLobbyState* m_lobbystate;
	friend class CState;
	//int substate;
};


