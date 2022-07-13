#pragma once
#include "CSession.h"
class CDBMgr
{

public:
	static CDBMgr* GetInst();
	static void Create();
	static void Destroy();

private:
	static CDBMgr* m_instance;
	CDBMgr();
	~CDBMgr();

public:
	void Init();
	void End();

	void SetJoin(list<t_UserInfo*> _users);
	list<t_UserInfo*> GetJoin();

	void InsertJointbl(t_UserInfo* _user);
	void InsertJoinLog(char* _content);

private:
	CLock* m_lock;

	MYSQL m_mysql;
	MYSQL_RES* m_sql_result;
	MYSQL_ROW m_sql_row;
	MYSQL_STMT* m_stmt_set;
};

