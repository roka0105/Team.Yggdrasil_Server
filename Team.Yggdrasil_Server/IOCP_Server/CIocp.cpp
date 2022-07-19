#include "pch.h"
#include "CIocp.h"
#include "CSessionMgr.h"
#include "CMainMgr.h"


DWORD CIocp::WorkThread(LPVOID _iocp)
{
	int retval;

	CIocp* ciocp = reinterpret_cast<CIocp*>(_iocp);

	while (1)
	{
		DWORD cbTransferred;
		SOCKET clientsock;
		OVERLAP_EX* overlap_ptr;
		void* session;
		retval = GetQueuedCompletionStatus(ciocp->m_hcp, &cbTransferred, &clientsock, (LPOVERLAPPED*)&overlap_ptr, INFINITE);

		bool check = ciocp->GetQueueErrorCheck(retval, cbTransferred, overlap_ptr);
		// check���� ����üũ�� �����߻��� type�� disconnected�� �ٲپ��ش�.
		switch (overlap_ptr->type)
		{
		case IO_TYPE::ACCEPT:
			session = ciocp->GetQueueAccept(clientsock, overlap_ptr);
			ciocp->Recv(session);
			break;
		case IO_TYPE::RECV:
			session = overlap_ptr->session;
			ciocp->SizeCheck_And_Recv(session, cbTransferred); // ���⼭ �Լ��ϳ��� ���� ó���ϵ���
			break;
		case IO_TYPE::SEND:
			session = overlap_ptr->session;
			ciocp->SizeCheck_And_Send(session, cbTransferred);
			break;
		case IO_TYPE::DISCONNECT:
			ciocp->DisConnect(overlap_ptr);
			break;
		}
	}

	return 0;
}


void CIocp::Init()
{
	m_hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hcp == NULL)
		exit(1);

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	HANDLE hThread;
	for (int i = 0; i < static_cast<int>(si.dwNumberOfProcessors * 2); i++)
	{
		hThread = CreateThread(NULL, 0, WorkThread, this, 0, NULL);
		if (hThread == NULL) exit(1);
		CloseHandle(hThread);
	}
}

void CIocp::End()
{

}

void CIocp::PostDisConnect(void* _ptr) // �ϴ� ����� �����ʰ�, ���� ����Ѵٸ� static���� ������ ������ WSAsend���� ����Ѵ�.
{
	OVERLAP_EX* tempoverlap = new OVERLAP_EX();
	tempoverlap->type = IO_TYPE::DISCONNECT;
	tempoverlap->session = _ptr;

	PostQueuedCompletionStatus(m_hcp, -1, NULL, (LPOVERLAPPED)tempoverlap);
}

bool CIocp::GetQueueErrorCheck(int _retval, int _cb_t, OVERLAP_EX* _overlapex)
{
	CSession* session = reinterpret_cast<CSession*>(_overlapex->session);

	if (_retval == 0 || _cb_t == 0) // �̰� ������ ���� �� ��쿡�� GEtQueueErrorcheck�� ȣ���Ѵ�.
	{
		if (_retval == 0)
		{
			DWORD temp1, temp2;
			WSAGetOverlappedResult(session->GetSock(), &_overlapex->overlapped, &temp1, FALSE, &temp2);
			err_display(_T("WSAGetOverlappedResult()"));
		}
		_overlapex->type = IO_TYPE::DISCONNECT;
		return true;
	}
	return false;
}

