#pragma once
class CLock;
class CLogMgr
{
public:
	static CLogMgr* GetInst();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	char* WriteLog(const char* fmt, ...);
	char* FileWriteLog(const char* fmt, ...);
	char* FileReadLogLast();
	char* FileReadLogAll();
private:
	CLogMgr();
	~CLogMgr();
private:
	CLock* m_lock;
	ifstream readFile;
	ofstream writeFile;
	static CLogMgr* m_instance;
	char m_logfilename[100];
	time_t timer;
	tm* t;
};

