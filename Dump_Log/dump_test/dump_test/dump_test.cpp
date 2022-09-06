#include "Exception_Handler.h"
#include "CLogMgr.h"
#include <atomic>
#include <tchar.h>
#include <wchar.h>
void create_exception(int* a = nullptr)
{
	a[100] = 100;
}
enum class EPROTOCOL
{
	NONE,
	MAX
};
static std::wstring EPROTOCOLtoString(EPROTOCOL _protocol)
{
	std::wstring type;
	switch (_protocol)
	{
	case EPROTOCOL::NONE:
		type = _T("None");
		break;
	case EPROTOCOL::MAX:
		type = _T("Max");
		break;
	}
	return type;
}
int main()
{
	CLogMgr::Create();
	CLogMgr::GetInst()->Init();

	Exception_Handler::Create();
	Exception_Handler::Instance()->_initalize(_T("test.dmp"));
	Exception_Handler::Instance()->run();

	std::wstring type = EPROTOCOLtoString(EPROTOCOL::NONE);
	CLogMgr::GetInst()->FileWriteLog(_T("%s"), type.c_str());

	LPBYTE test=nullptr;
	*test = 12345;
}

