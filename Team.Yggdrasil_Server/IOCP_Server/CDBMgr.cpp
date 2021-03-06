#include "pch.h"
#include "CDBMgr.h"
#include "CLoginMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "CLogMgr.h"

CDBMgr* CDBMgr::m_instance = nullptr;

CDBMgr* CDBMgr::GetInst()
{
	return m_instance;
}

void CDBMgr::Create()
{
	if (m_instance == nullptr)
		m_instance = new CDBMgr();
}

void CDBMgr::Destroy()
{
	delete m_instance;
}

CDBMgr::CDBMgr()
{
	m_lock = new CLock();
}

CDBMgr::~CDBMgr()
{
	delete m_lock;
}

void CDBMgr::Init()
{
	mysql_init(&m_mysql);

	if (!mysql_real_connect(&m_mysql, HOST_IP, USER, PASSWORD, DATABASE, 3306, NULL, 0))
	{
		std::cout << "mysql connected error : " << mysql_error(&m_mysql) << std::endl;
		exit(-1);
	}
	mysql_query(&m_mysql, "set names euckr;");

	CLoginMgr::GetInst()->SetJoinlist(GetJoin());
}

void CDBMgr::End()
{
	CLogMgr::GetInst()->FileWriteLog(_T("db end start\n"));
	CLogMgr::GetInst()->FileWriteLog(_T("&m_mysql: %p\n"), &m_mysql);
	mysql_close(&m_mysql);
	const char* test = mysql_error(&m_mysql);
	CLogMgr::GetInst()->FileWriteLog(_T("db end\n"));
}

void CDBMgr::SetJoin(list<t_UserInfo*> _users)
{
	CLockGuard<CLock> lock(m_lock);
	char temp[100];
	ZeroMemory(temp, 100);
	//테이블 삭제 및 생성 쿼리문 실행 후
	sprintf(temp, "truncate table jointbl" /* table이름 */);
	if (mysql_query(&m_mysql, temp))
	{
		printf("** %s **\n", mysql_error(&m_mysql));
	}
	ZeroMemory(temp, 100);
	//db에 넣는다.
	for (t_UserInfo* user : _users)
	{
		sprintf(temp, "insert into jointbl values('%s','%s','%s');", user->id, user->pw, user->nickname);
		if (mysql_query(&m_mysql, temp))
		{
			printf("** %s **\n", mysql_error(&m_mysql));
		}
	}
}

list<t_UserInfo*> CDBMgr::GetJoin()
{
	CLockGuard<CLock> lock(m_lock);
	const char* query;
	char temp[100];
	ZeroMemory(temp, 100);
	sprintf(temp, "select * from jointbl");
	query = temp;

	if (mysql_query(&m_mysql, query))
	{
		printf("** %s **\n", mysql_error(&m_mysql));
	}
	m_sql_result = mysql_store_result(&m_mysql);

	list<t_UserInfo*>users;
	TCHAR ID[STRINGSIZE], PW[STRINGSIZE], NICK[STRINGSIZE];
	
	while ((m_sql_row = mysql_fetch_row(m_sql_result)) != NULL)
	{
		ZeroMemory(ID, STRINGSIZE);
		ZeroMemory(PW, STRINGSIZE);
		ZeroMemory(NICK, STRINGSIZE);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_sql_row[0], strlen(m_sql_row[0]), ID, STRINGSIZE);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_sql_row[1], strlen(m_sql_row[1]), PW, STRINGSIZE);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_sql_row[2], strlen(m_sql_row[2]), NICK, STRINGSIZE);
		t_UserInfo* user = new t_UserInfo(ID, PW, NICK);
		users.push_back(user);
	}
	mysql_free_result(m_sql_result);
	return users;
}
void CDBMgr::InsertJointbl(t_UserInfo* _user)
{
	CLockGuard<CLock> lock(m_lock);
	char query[100];
	ZeroMemory(query, 100);
	TCHAR temp[100];
	ZeroMemory(temp, 100);
	_stprintf(temp, _T("insert into jointbl values('%s','%s','%s');"), _user->id, _user->pw, _user->nickname);
	WideCharToMultiByte(CP_ACP, 0, temp, -1, query, 100, NULL, NULL);
	if (mysql_query(&m_mysql, (char*)query))
	{
		printf("** %s **\n", mysql_error(&m_mysql));
	}
}
void CDBMgr::InsertJoinLog(TCHAR* _content)
{
	CLockGuard<CLock> lock(m_lock);
	char query[BUFSIZE];
	TCHAR temp[BUFSIZE];
	ZeroMemory(temp, BUFSIZE);
	ZeroMemory(query, BUFSIZE);

	_stprintf(temp, _T("insert into joinlogtbl values(null, curdate(), curtime(), '%s')"), _content);
	WideCharToMultiByte(CP_ACP, 0, temp, -1, query, BUFSIZE, NULL, NULL);
	if (mysql_query(&m_mysql, query))
	{
		printf("** %s **\n", mysql_error(&m_mysql));
	}
}

