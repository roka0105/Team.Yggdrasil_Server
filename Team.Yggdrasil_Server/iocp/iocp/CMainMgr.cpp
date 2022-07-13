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
			err_display("accept()");
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

	InitializeCriticalSection(&m_cs);

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
	printf("end!!!!!\n");
	CLoginMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog("itewyhfdrihtoeriyt\n");
	CSessionMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog("itewyhfdrihtoeriyt\n");
	CDBMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog("itewyhfdrihtoeriyt\n");
	CLogMgr::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog("itewyhfdrihtoeriyt\n");
	C_SetCtrlHandler::GetInst()->End();
	CLogMgr::GetInst()->FileWriteLog("마지막\n");
	CLobbyMgr::GetInst()->End();
	printf("end!!!!!\n");
	DeleteCriticalSection(&m_cs);
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
	CLogMgr::GetInst()->FileWriteLog("itewyhfdrihtoeriyt\n");
	CSessionMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog("itewyhfdrihtoeriyt\n");
	CDBMgr::Destroy();
	CLogMgr::GetInst()->FileWriteLog("itewyhfdrihtoeriyt\n");
	C_SetCtrlHandler::Destroy();
	CLogMgr::GetInst()->FileWriteLog("마지막\n");
	CLogMgr::Destroy();
	CProtocolMgr::Destroy();
	CLobbyMgr::Destroy();
}

void CMainMgr::SizeCheck_And_Recv(void* _session, int _combytes) // 이름을 하는 기능을 전부 명시적으로 만든다.
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
	// 클라이언트 나감처리
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
	// 여기서 session recv를 호출
	return session;
}



void CMainMgr::RecvProcess(CSession* _ptr)
{
	/*switch (_ptr->GetState())
	{
	case STATE::MENU_SELECT:
		CLoginMgr::GetInst()->MenuProc(_ptr);
		break;
	case STATE::JOIN:
		CLoginMgr::GetInst()->JoinProc(_ptr);
		break;
	case STATE::LOGIN:
		CLoginMgr::GetInst()->LoginProc(_ptr);
		break;
	case STATE::LOGOUT:
		CLoginMgr::GetInst()->LogOutProc(_ptr);
		break;
	}*/
}

void CMainMgr::SendProcess(CSession* _ptr)
{
	// 처음 state가 menu_select일 때 singel에 따라서 state를 변경시키도록 하였다.
	// 그러나 state::

	//switch (_ptr->GetState())
	//{
	//case STATE::LOGIN:
	//	_ptr->SetState(STATE::MENU_SELECT);
	//	break;
	//case STATE::JOIN:
	//	_ptr->SetState(STATE::MENU_SELECT);
	//	break;
	////case STATE::LOGOUT:

	//}

}
