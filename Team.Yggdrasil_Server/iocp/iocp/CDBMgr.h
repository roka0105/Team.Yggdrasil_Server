#pragma once
#include "CSession.h"
class CLock;
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
	void InsertJoinLog(TCHAR* _content);
private:
	CLock* m_lock;
	MYSQL m_mysql;
	MYSQL_RES* m_sql_result;
	MYSQL_ROW m_sql_row;
	MYSQL_STMT* m_stmt_set;
};

//방법 1 csv 파일을 읽어와서 db에 데이터 업로드 

