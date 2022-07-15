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

/*2022 07 15 작업 내역 
  방리스트 모두 보내는 프로토콜과 함수 구현
  방리스트 임의의 페이지에 해당하는 방만 보내는 프로토콜과 함수 구현
  내일 할것 클라랑 연동하기
  그리고 내 생각인데 subprotocol에 lobbyenter chatsend 이런거를 디테일로 내리고
  multi와 single를 subprotocol에 넣는게 더 이쁠려나..?
  왜 그렇게 생각하냐면 지금 구상대로면 multi에서는 createroom,sendchat을 쓰겠지만 
  single에서는 안쓰기 때문에 Lobbyenter밖에 안씀. multi에서 쓰는기능 single에서 쓰는기능이
  한눈에 파악이 가능할 듯.
*/

/*
내일 할일
1.lock 사용하도록 짜놓기
2.유니코드로 바꾸고 다른 데이터들 char[] 문자열 -> tchar,char* -> byte로 바꿔놓기
3.logwritelast 수정하기.
*/