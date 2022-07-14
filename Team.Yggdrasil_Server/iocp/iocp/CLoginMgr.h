#pragma once

class CSession;
class CLock;
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
	BOOL LoginCheck(TCHAR* _id, TCHAR* _pw, TCHAR* _nick);
	//-------------------

	//---- join func ----
	BOOL joinCheck(TCHAR* _msg, TCHAR* _id, TCHAR* _nick);
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

	unsigned long GetProtocol(byte* _recvbuf);
	void Packing(byte* _sendbuf, unsigned long _protocol, const TCHAR* _id, const TCHAR* _pw, CSession* _ptr);
	void Packing(byte* _sendbuf, unsigned long _protocol, const TCHAR* _str, CSession* _ptr);
	void Packing(byte* _sendbuf, unsigned long _protocol, bool _flag, const TCHAR* _str, CSession* _ptr);

	void UnPacking(const byte* _recvbuf, TCHAR* _id, TCHAR* _pw);
	void UnPacking(const byte* _recvbuf, TCHAR* _id, TCHAR* _pw, TCHAR* _nickname);
						
	BOOL SearchFile(const TCHAR* filename);
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
	CLock* m_lock;
	list<t_UserInfo*> m_loginlist;
	list<t_UserInfo*> m_joinlist;
};

