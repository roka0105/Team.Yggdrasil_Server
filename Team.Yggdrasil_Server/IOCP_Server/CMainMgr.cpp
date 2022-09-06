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
#include "CRoomMgr.h"
#include "CSectorMgr.h"
#include "CGameMgr.h"
#include "Exception_Handler.h"

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
	Exception_Handler::Instance()->run();
	while (1)
	{
		SOCKET clientsock;
		clientsock = listen_sock->Accept();
		if (clientsock == INVALID_SOCKET)
		{
			err_display(_T("accept()"));
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
	CRoomMgr::GetInst()->Init();
	CGameMgr::GetInst()->Init();
	Exception_Handler::Instance()->_initalize(_T("server_exception.dmp"));
	// ++
}
void CMainMgr::End()
{

	CLoginMgr::GetInst()->End();
	CLobbyMgr::GetInst()->End();
	CRoomMgr::GetInst()->End();
	CSessionMgr::GetInst()->End();
	CDBMgr::GetInst()->End();
	C_SetCtrlHandler::GetInst()->End();
	CLogMgr::GetInst()->End();
	CGameMgr::GetInst()->End();

	MemoryPool_2::End();
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
	CRoomMgr::Create();
	CGameMgr::Create();
	Exception_Handler::Create();
}

void CMainMgr::Destroy()
{
	delete m_instance;
	CLoginMgr::Destroy();
	CSessionMgr::Destroy();
	CDBMgr::Destroy();
	C_SetCtrlHandler::Destroy();
	CLogMgr::Destroy();
	CProtocolMgr::Destroy();
	CLobbyMgr::Destroy();
	CRoomMgr::Destroy();
	CGameMgr::Destroy();
	Exception_Handler::Destroy();
}

void CMainMgr::SizeCheck_And_Recv(void* _session, int _combytes, t_ThreadInfo* _threadinfo) // 이름을 하는 기능을 전부 명시적으로 만든다.
{
	CSession* session = reinterpret_cast<CSession*>(_session);
	SOC sizecheck = session->CompRecv(_combytes);

	switch (sizecheck)
	{
	case SOC::SOC_TRUE:
		//recvprocess
		session->GetState()->Recv(_threadinfo);
		break;
	case SOC::SOC_FALSE:
		break;
	}

	session->WSARECV();
}

void CMainMgr::SizeCheck_And_Send(void* _session, int _combytes, t_ThreadInfo* _threadinfo)
{
	CSession* session = reinterpret_cast<CSession*>(_session);
	SOC sizecheck = session->CompSend(_combytes);
	switch (sizecheck)
	{
	case SOC::SOC_TRUE:
		//SendProcess(session);
		session->GetState()->Send(_threadinfo);
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
	CSession* ptr = reinterpret_cast<CSession*>(_session);
	return ptr->WSASEND();
}

BOOL CMainMgr::Recv(void* _session)
{
	CSession* ptr = reinterpret_cast<CSession*>(_session);
	return ptr->WSARECV();
}

int CMainMgr::DisConnect(OVERLAP_EX* _overlap)
{
	OVERLAP_EX* overlap = reinterpret_cast<OVERLAP_EX*>(_overlap);
	CSession* session = reinterpret_cast<CSession*> (overlap->session);
	overlap->session = nullptr;

	t_UserInfo* userinfo = session->GetUserInfo();
	//로그인중이면 로그아웃 처리
	if (userinfo->is_login)
	{
		CLoginMgr::GetInst()->RemoveLogingInfo(userinfo);
	}
	CState* state = session->GetState();
	if (dynamic_cast<CLobbyState*>(state) != nullptr)
	{
		CLobbyMgr::GetInst()->RemoveSession(session);
	}
	else if (dynamic_cast<CRoomState*>(state) != nullptr)
	{
		CRoomMgr::GetInst()->DisConnected(session);
	}
	/*printf("%s ,%s\n", typeid(dynamic_cast<CLobbyState*>(state)).name(), typeid(CLobbyState*).name());
	if (typeid(dynamic_cast<CLobbyState*>(state))== typeid(CLobbyState*))
	{
		CLobbyMgr::GetInst()->RemoveSession(session);
	}
	*/
	C_SetCtrlHandler::GetInst()->End();

	// 클라이언트 나감처리
	CSessionMgr::GetInst()->RemoveSession(session);
	return 0;
}

void* CMainMgr::GetQueueAccept(ULONG_PTR _com_key, OVERLAP_EX* _overlap)
{
	delete _overlap;
	SOCKET session_sock = static_cast<SOCKET>(_com_key);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(session_sock), m_hcp, session_sock, 0);
	CSession* session = CSessionMgr::GetInst()->AddSession(session_sock);
	// 여기서 session recv를 호출
	return session;
}
