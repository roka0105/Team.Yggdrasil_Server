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

/*
���� ����
1.db ���� ���ڵ� ������� �������� �Լ� �����. (proc)
2.join ������Ʈ �ɶ����� �ﰢ������ db�� ���ε�
3.logwritelast �����ϱ�.
*/