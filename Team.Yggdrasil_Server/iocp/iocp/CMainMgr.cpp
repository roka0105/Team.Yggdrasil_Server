#include "pch.h"
#include "CMainMgr.h"
#include "CSession.h"
#include "CSessionMgr.h"
#include "CLoginMgr.h"
#include "CSocket.h"
#include "CDBMgr.h"
#include "CLogMgr.h"
#include "C_SetCtrlHandler.h"
#include "CProtocolMgr.h"
#include "CLobbyMgr.h"

CMainMgr* CMainMgr::m_instance = nullptr;

CMainMgr::CMainMgr()
{

}
CMainMgr::~CMainMgr()
{

}
void CMainMgr::Loop()
{

	Init();

	Run();

	End();
}
void CMainMgr::Run()
{
	while (1)
	{
		SOCKET clientsock;
		clientsock = listen_sock->Accept();
		if (clientsock == INVALID_SOCKET)
		{
			err_display(L"accept()");
			return;
		}

		PostQueueAccept(clientsock);
	
	}

}
void CMainMgr::Init()
{


	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
		exit(1);

	listen_sock = new CSocket(9000);

	CIocp::Init();
	CLoginMgr::GetInst()->Init();
	CDBMgr::GetInst()->Init();
	CLogMgr::GetInst()->Init();
	C_SetCtrlHandler::GetInst()->Init();
	CLobbyMgr::GetInst()->Init();
	// ++
}
void CMainMgr::End()
{
	
	CLoginMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog(L"end1\n");
	CLobbyMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog(L"end2\n");
	CSessionMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog(L"end3\n");
	CDBMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog(L"end4\n");
	
	C_SetCtrlHandler::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog(L"end5\n");
	
	CLogMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog(L"end6\n");
	
	WSACleanup();
}
CMainMgr* CMainMgr::GetInst()
{
	return m_instance;
}

void CMainMgr::Create()
{
	if (m_instance == nullptr)
		m_instance = new CMainMgr();
	CLoginMgr::Create();
	CSessionMgr::Create();
	CDBMgr::Create();
	CLogMgr::Create();
	C_SetCtrlHandler::Create();
	CProtocolMgr::Create();
	CLobbyMgr::Create();
}

void CMainMgr::Destroy()
{
	delete m_instance;
	CLoginMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog(L"destroy1\n");
	CSessionMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog(L"destroy2\n");
	CDBMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog(L"destroy3\n");
	C_SetCtrlHandler::Destroy();
	CLogMgr::GetInst()->FileWriteLog(L"destroy4\n");
	CLogMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog(L"destroy5\n");
	CProtocolMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog(L"destroy6\n");
	CLobbyMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog(L"destroy last\n");
}

void CMainMgr::SizeCheck_And_Recv(void* _session, int _combytes) // �̸��� �ϴ� ����� ���� ��������� �����.
{
	CSession* session = (CSession*)_session;
	SOC sizecheck = session->CompRecv(_combytes);

	switch (sizecheck)
	{
	case SOC::SOC_TRUE:
		//recvprocess
		session->GetState()->Recv();
		break;
	case SOC::SOC_FALSE:
		break;
	}

	session->WSARECV();
}

void CMainMgr::SizeCheck_And_Send(void* _session, int _combytes)
{
	CSession* session = (CSession*)_session;
	SOC sizecheck = session->CompSend(_combytes);
	switch (sizecheck)
	{
	case SOC::SOC_TRUE:
		//SendProcess(session);
		session->GetState()->Send();
		session->DelayDataSend();
		break;
	case SOC::SOC_FALSE:
		session->WSASEND();
		break;
	}
}


void CMainMgr::PostQueueAccept(SOCKET _clientsock)
{
	OVERLAP_EX* tempoverlap = new OVERLAP_EX();
	tempoverlap->type = IO_TYPE::ACCEPT;
	PostQueuedCompletionStatus(m_hcp, -1, _clientsock, (LPOVERLAPPED)tempoverlap);
}

void CMainMgr::PostDisConnect(CSession* _ptr)
{
	OVERLAP_EX* tempoverlap = new OVERLAP_EX();
	tempoverlap->type = IO_TYPE::DISCONNECT;
	tempoverlap->session = _ptr;

	PostQueuedCompletionStatus(m_hcp, -1, NULL, (LPOVERLAPPED)tempoverlap);
}



BOOL CMainMgr::Send(void* _session)
{
	CSession* ptr = (CSession*)_session;
	return ptr->WSASEND();
}

BOOL CMainMgr::Recv(void* _session)
{
	CSession* ptr = (CSession*)_session;
	return ptr->WSARECV();
}

int CMainMgr::DisConnect(OVERLAP_EX* _overlap)
{
	OVERLAP_EX* overlap = (OVERLAP_EX*)_overlap;
	CSession* session = (CSession*)overlap->session;
	overlap->session = nullptr;
	// Ŭ���̾�Ʈ ����ó��
	CSessionMgr::GetInst()->RemoveSession(session);
	C_SetCtrlHandler::GetInst()->End();
	return 0;
}

void* CMainMgr::GetQueueAccept(ULONG_PTR _com_key, OVERLAP_EX* _overlap)
{
	delete _overlap;
	SOCKET session_sock = (SOCKET)_com_key;
	CreateIoCompletionPort((HANDLE)session_sock, m_hcp, session_sock, 0);
	CSession* session = CSessionMgr::GetInst()->AddSession(session_sock);
	// ���⼭ session recv�� ȣ��
	return session;
}
