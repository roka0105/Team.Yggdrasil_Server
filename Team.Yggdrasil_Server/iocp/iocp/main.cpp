#include "pch.h"
#include "CMainMgr.h"

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtDumpMemoryLeaks();
	CMainMgr::Create();
	CMainMgr::GetInst()->Loop();
	CMainMgr::Destroy();
	return 0;
}
/*이번주 할 일
1. 로비 구현 (방만들기,전체채팅,방목록출력)
2. */

/*
내일 할일
1.lock 사용하도록 짜놓기
2.유니코드로 바꾸고 다른 데이터들 char[] 문자열 -> tchar,char* -> byte로 바꿔놓기
3.logwritelast 수정하기.
*/