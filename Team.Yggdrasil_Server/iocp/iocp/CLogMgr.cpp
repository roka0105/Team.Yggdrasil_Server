#include "pch.h"
#include "CLogMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
CLogMgr* CLogMgr::m_instance = nullptr;

CLogMgr* CLogMgr::GetInst()
{
	return m_instance;
}

void CLogMgr::Create()
{
	if (m_instance == nullptr)
		m_instance = new CLogMgr();
}

void CLogMgr::Destroy()
{
	delete m_instance;
}

void CLogMgr::Init()
{ 
	timer = time(NULL);
	t = localtime(&timer);
	_stprintf(m_logfilename, L"[%d_%d_%d]  Log.txt", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	readFile.open(m_logfilename);
}

void CLogMgr::End()
{
	readFile.close();
}

TCHAR* CLogMgr::WriteLog(const TCHAR* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	TCHAR cbuf[BUFSIZE];
	ZeroMemory(cbuf, BUFSIZE);
	t = localtime(&timer);
	_stprintf(cbuf,L"날짜:%d년%d월%d일 시간: %d시 %d분 ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	int size = _tcslen(cbuf);
	_vstprintf(cbuf+size, fmt, arg);
	printf("%s", cbuf);

	va_end(arg);
	return cbuf;
}
TCHAR* CLogMgr::FileWriteLog(const TCHAR* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	TCHAR cbuf[BUFSIZE];
	ZeroMemory(cbuf, BUFSIZE);
	t = localtime(&timer);
	_stprintf(cbuf, L"[날짜:%d년%d월%d일] [시간: %d시 %d분] ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	int size = _tcslen(cbuf);
	_vstprintf(cbuf + size, fmt, arg);
	va_end(arg);
	_stprintf(m_logfilename, L"[%d_%d_%d]  Log.txt", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	
	CLock_Guard<CLock> lock(m_lock);
	
	writeFile.open(m_logfilename,std::ios::app);
	size = _tcslen(cbuf);
	writeFile.write(cbuf, size);
	writeFile.close();

	return cbuf;
}
//고치기 안나옴
TCHAR* CLogMgr::FileReadLogLast()
{
	TCHAR temp[BUFSIZE];
	ZeroMemory(temp, BUFSIZE);
	t = localtime(&timer);
	_stprintf(m_logfilename, L"[%d_%d_%d]  Log.txt", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	
	CLock_Guard<CLock> lock(m_lock);
	
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			readFile.getline(temp, BUFSIZE);
		}
		_tprintf(L"%s\n", temp);
		readFile.clear();
		readFile.seekg(0, std::ios_base::beg);
	}
	return 0;
}
TCHAR* CLogMgr::FileReadLogAll()
{
	CLock_Guard<CLock> lock(m_lock);
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			TCHAR temp[BUFSIZE];
			ZeroMemory(temp, BUFSIZE);
			readFile.getline(temp, BUFSIZE);
			_tprintf(:"%s\n", temp);
		}
		readFile.clear();
		readFile.seekg(0, std::ios_base::beg);
	}
	return 0;
}
CLogMgr::CLogMgr()
{
	m_lock = new CLock();
}

CLogMgr::~CLogMgr()
{
	delete m_lock;
}

void err_quit(const TCHAR* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,
		0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_OK);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void err_display(const TCHAR* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,
		0, NULL);
	_tprintf(L"[%s] %s", msg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}