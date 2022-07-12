#pragma once

class CSession;

#include "CSession.h"

class CLoginMgr
{
public:
	static CLoginMgr* GetInst();
	static void Create();
	static void Destroy();

private:
	static CLoginMgr* m_instance;

	CLoginMgr();
	~CLoginMgr();

public:
	void Init();
	void End();

	void LoginProcess(CSession* _ptr);
	void EnterLobbyProcess(CSession* _ptr);
	void LoginFunc(CSession* _ptr);
	void JoinFunc(CSession* _ptr);
	void LogOutFunc(CSession* _ptr);
private:
	//---- login func----
	BOOL LoginCheck(char* _id, char* _pw, char* _nick);
	//-------------------

	//---- join func ----
	BOOL joinCheck(char* _msg, char* _id, char* _nick);
	//-------------------

public:
	
	enum class SUBPROTOCOL
	{
		NONE,
		LoginInfo,
		LoginResult,
		JoinInfo,
		JoinResult,
		LogoutInfo,
		LogoutResult,
		LobbyEnter,
		LobbyResult,
		MAX
	};

	unsigned long GetProtocol(char* _recvbuf);
	void Packing(char* _sendbuf, unsigned long _protocol, const char* _id, const char* _pw, CSession* _ptr);
	void Packing(char* _sendbuf, unsigned long _protocol, const char* _str, CSession* _ptr);
	void Packing(char* _sendbuf, unsigned long _protocol, bool _flag, const char* _str, CSession* _ptr);

	void UnPacking(const char* _recvbuf, char* _id, char* _pw);
	void UnPacking(const char* _recvbuf, char* _id, char* _pw, char* _nickname);

	BOOL SearchFile(const char* filename);
	bool FileDataLoad();
	bool FileDataAdd(t_UserInfo* _info);


	void SetJoinlist(list<t_UserInfo*> _list) 
	{ 
		for (t_UserInfo* item : _list)
		{
			m_joinlist.push_back(item);
		}
	}

private:
	list<t_UserInfo*> m_loginlist;
	list<t_UserInfo*> m_joinlist;
};

