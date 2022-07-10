#include "pch.h"
#include "CDBMgr.h"
#include "CLoginMgr.h"

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
}

CDBMgr::~CDBMgr()
{
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
	m_stmt_set = mysql_stmt_init(&m_mysql);

	CLoginMgr::GetInst()->SetJoinlist(GetJoin());
}

void CDBMgr::End()
{
	mysql_stmt_close(m_stmt_set);
	mysql_close(&m_mysql);
}

void CDBMgr::SetJoin(list<t_UserInfo*> _users)
{
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
	char ID[IDSIZE], PW[PWSIZE], NICK[NAMESIZE];
	ZeroMemory(ID, IDSIZE);
	ZeroMemory(PW, PWSIZE);
	ZeroMemory(NICK, NAMESIZE);
	while ((m_sql_row = mysql_fetch_row(m_sql_result)) != NULL)
	{
		memcpy(ID, m_sql_row[0], sizeof(m_sql_row[0]));
		memcpy(PW, m_sql_row[1], sizeof(m_sql_row[1]));
		memcpy(NICK, m_sql_row[2], sizeof(m_sql_row[2]));
		t_UserInfo* user = new t_UserInfo(ID, PW, NICK);
		users.push_back(user);
	}

	//int rowcount = mysql_num_rows(m_sql_result);
	//ZeroMemory(m_sql_row, sizeof(MYSQL_ROW) * rowcount);
	mysql_free_result(m_sql_result);
	return users;
}

void CDBMgr::InsertJoinLog(char* _content)
{
	const char* query;
	char temp[BUFSIZE];
	ZeroMemory(temp, BUFSIZE);
	sprintf(temp, "insert into joinlogtbl values(null, curdate(), curtime(), '%s')", _content);
	query = temp;
	if (mysql_query(&m_mysql, query))
	{
		printf("** %s **\n", mysql_error(&m_mysql));
	}
}

