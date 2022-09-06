#pragma once
#include "pch.h"

#pragma warning(push)
#pragma warning(disable : 4091)
#include <DbgHelp.h>
#pragma warning(pop)
#include "CLock.h"
#include "CLockGuard.h"

#pragma comment(lib,"Dbghelp.lib")

class Exception_Handler
{
private:
	static Exception_Handler* volatile m_instance;
	static CLock* m_lock;

	Exception_Handler() {}
	Exception_Handler(const Exception_Handler& other);
	~Exception_Handler(){}
public:
	static void volatile Create()
	{
		if (m_instance == nullptr)
		{
			CLockGuard<CLock> lock(m_lock);
			if (m_instance == nullptr)
			{
				m_instance = new Exception_Handler();
			}
		}
	}
	static Exception_Handler* volatile Instance()
	{
		return m_instance;
	}
	static void volatile Destroy()
	{
		delete m_instance;
		delete m_lock;
	}

public:
	DWORD _initalize(__in LPCTSTR dump_file_name,
		__in const MINIDUMP_TYPE dump_type = MINIDUMP_TYPE::MiniDumpNormal);
	DWORD run();
	static LONG WINAPI Exception_Callback(__in struct _EXCEPTION_POINTERS* exceptioninfo);
	
private:
	wstring  _dump_file_name;
	MINIDUMP_TYPE _dump_type;
	LPTOP_LEVEL_EXCEPTION_FILTER _prev_filter;
};

static wstring IOTYPEtoString(IO_TYPE _type);
static wstring ESTATEtoString(E_STATE _state);
static void PROTOCOLtoString(unsigned long& original,E_STATE& curstate,wstring* outProtocol_1, wstring* outProtocol_2, wstring* outProtocol_3);
