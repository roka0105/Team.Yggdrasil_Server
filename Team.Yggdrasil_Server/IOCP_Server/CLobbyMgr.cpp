#include "pch.h"
#include "CLobbyMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "CRoomMgr.h"
#include "CProtocolMgr.h"

CLobbyMgr* CLobbyMgr::instance = nullptr;
CLobbyMgr::CLobbyMgr()
{
	m_lock = new CLock();
}
CLobbyMgr::~CLobbyMgr()
{
	delete m_lock;
}

void CLobbyMgr::UnPacking(byte* _recvdata, TCHAR* msg)
{
	byte* ptr = _recvdata;
	int strsize = 0;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(msg, ptr, strsize * CODESIZE);
}

void CLobbyMgr::Packing(unsigned long _protocol, TCHAR* msg, CSession* _session)
{
	byte senddata[BUFSIZE];
	ZeroMemory(senddata, BUFSIZE);
	byte* ptr = senddata;
	int size = 0;
	int strsize = _tcslen(msg) * CODESIZE;
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, msg, strsize);
	size += strsize;

	_session->Packing(_protocol, senddata, size);
}

void CLobbyMgr::Packing(unsigned long _protocol, bool result, CSession* _session)
{
	byte senddata[BUFSIZE];
	ZeroMemory(senddata, BUFSIZE);
	byte* ptr = senddata;
	int size = 0;
	memcpy(ptr, &result,sizeof(bool));
	size += sizeof(bool);

	_session->Packing(_protocol, senddata, size);
}

CLobbyMgr* CLobbyMgr::GetInst()
{
	return instance;
}

void CLobbyMgr::Create()
{
	if (instance == nullptr)
		instance = new CLobbyMgr();
}

void CLobbyMgr::Destroy()
{
	delete instance;
}

void CLobbyMgr::Init()
{
}

void CLobbyMgr::End()
{
}

void CLobbyMgr::LobbyProcess(CSession* _session)
{
	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	unsigned long detailprotocol = CProtocolMgr::GetInst()->GetDetailProtocol(protocol);
	//switch subprotocol
	switch (static_cast<SUBPROTOCOL>(subprotocol))
	{
	case SUBPROTOCOL::Multi:
	{

		switch (detailprotocol)
		{
		case static_cast<const unsigned long>(DETAILPROTOCOL::CreateRoom):
			CreateRoomFunc(_session);
			break;
		case (static_cast<const unsigned long>(DETAILPROTOCOL::RoomlistUpdate) | static_cast<const unsigned long>(DETAILPROTOCOL::PageRoom)):
			PageRoomFunc(_session);
			break;
		case (static_cast<const unsigned long>(DETAILPROTOCOL::RoomlistUpdate) | static_cast<const unsigned long>(DETAILPROTOCOL::AllRoom)):
			//AllRoomFunc(_session);
			break;
		case (static_cast<const unsigned long>(DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(DETAILPROTOCOL::AllMsg)):
			ChattingFunc(_session);
			break;
		case (static_cast<const unsigned long>(DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(DETAILPROTOCOL::NoticeMsg)):
			break;
		}
		break;
	}
	case SUBPROTOCOL::Single:
		switch (detailprotocol)
		{

		}
		break;

	}
}

void CLobbyMgr::EnterRoomProcess(CSession* _session)
{
	//방입장 가능 여부 체크 
	//방입장 가능시 state room으로 변경.
	//전송 프로토콜 room , enterroomresult
}

void CLobbyMgr::BackPageProcess(CSession* _session)
{
	//state ->login 변경 
	unsigned long protocol = 0;
	_session->UnPacking(protocol);
	unsigned long subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	switch (static_cast<SUBPROTOCOL>(subprotocol))
	{
	case SUBPROTOCOL::BackPage:

		break;
	}
}

//void CLobbyMgr::AllRoomFunc(CSession* _session)
//{
//	CLockGuard<CLock> lock(m_lock);
//	CRoomMgr::GetInst()->SendRoom(_session);
//}

void CLobbyMgr::CreateRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	//방생성 가능 여부 체크 (실패의 경우가 생각이 안남)
	//방 생성 결과 전송
	bool result = true;
	if (result)
	{
		CRoomMgr::GetInst()->AddRoom(_session);
		_session->SetState(_session->GetRoomState());
	}
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::LOBBY));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::Multi));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(DETAILPROTOCOL::CreateRoomResult));

	Packing(protocol, result, _session);
}

void CLobbyMgr::PageRoomFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	unsigned int page = 0;

	//page unpack
	CRoomMgr::GetInst()->SendRoom(page, _session);
}

void CLobbyMgr::ChattingFunc(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	TCHAR msg[BUFSIZE];
	ZeroMemory(msg, BUFSIZE);
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	UnPacking(data, msg);
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, (unsigned long)MAINPROTOCOL::LOBBY);
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, (unsigned long)SUBPROTOCOL::Multi);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)DETAILPROTOCOL::ChatRecv);
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, (unsigned long)DETAILPROTOCOL::AllMsg);

	for (CSession* client : m_lobby_session_list)
	{
		Packing(protocol, msg, client);
	}
	_tprintf(_T("%s"), msg);

}

void CLobbyMgr::AddLobbySession(CSession* _session)
{
	CLockGuard<CLock> lock(m_lock);
	m_lobby_session_list.push_back(_session);
}



