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
/*�̹��� �� ��
1. �κ� ���� (�游���,��üä��,�������)
2. */

/*2022 07 15 �۾� ���� 
  �渮��Ʈ ��� ������ �������ݰ� �Լ� ����
  �渮��Ʈ ������ �������� �ش��ϴ� �游 ������ �������ݰ� �Լ� ����
  ���� �Ұ� Ŭ��� �����ϱ�
  �׸��� �� �����ε� subprotocol�� lobbyenter chatsend �̷��Ÿ� �����Ϸ� ������
  multi�� single�� subprotocol�� �ִ°� �� �̻ܷ���..?
  �� �׷��� �����ϳĸ� ���� �����θ� multi������ createroom,sendchat�� �������� 
  single������ �Ⱦ��� ������ Lobbyenter�ۿ� �Ⱦ�. multi���� ���±�� single���� ���±����
  �Ѵ��� �ľ��� ������ ��.
*/

/*
���� ����
1.lock ����ϵ��� ¥����
2.�����ڵ�� �ٲٰ� �ٸ� �����͵� char[] ���ڿ� -> tchar,char* -> byte�� �ٲ����
3.logwritelast �����ϱ�.
*/