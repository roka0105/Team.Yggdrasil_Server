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
	sprintf(m_logfilename, "[%d_%d_%d]  Log.txt", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	writeFile.open(m_logfilename, std::ios::app);
	writeFile.close();
	readFile.open(m_logfilename);
	setlocale(LC_ALL, "korean");
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
	_tprintf(L"%s", cbuf);

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
	sprintf(m_logfilename, "[%d_%d_%d]  Log.txt", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	
	CLockGuard<CLock> lock(m_lock);
	char data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	writeFile.open(m_logfilename,std::ios::app);
	size = _tcslen(cbuf);
	WideCharToMultiByte(CP_ACP, 0, cbuf, -1, data, BUFSIZE, NULL, NULL);
	writeFile.write(data, strlen(data));
	writeFile.close();

	return cbuf;
}
//고치기 안나옴
TCHAR* CLogMgr::FileReadLogLast()
{
	char msg[BUFSIZE];
	ZeroMemory(msg, BUFSIZE);
	TCHAR temp[BUFSIZE];
	ZeroMemory(temp, BUFSIZE);
	t = localtime(&timer);
	sprintf(m_logfilename, "[%d_%d_%d]  Log.txt", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	
	CLockGuard<CLock> lock(m_lock);
	
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			readFile.getline(msg, BUFSIZE);
		}
		MultiByteToWideChar(CP_ACP, 0, msg, -1, temp, BUFSIZE);
		_tprintf(L"%s\n", temp);
		readFile.clear();
		readFile.seekg(0, std::ios_base::beg);
	}
	return 0;
}
TCHAR* CLogMgr::FileReadLogAll()
{
	CLockGuard<CLock> lock(m_lock);
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			TCHAR temp[BUFSIZE];
			char msg[BUFSIZE];
			ZeroMemory(msg, BUFSIZE);
			ZeroMemory(temp, BUFSIZE);
			readFile.getline(msg, BUFSIZE);
			MultiByteToWideChar(CP_ACP, 0, msg, -1, temp, BUFSIZE);
			_tprintf(L"%s\n", temp);
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