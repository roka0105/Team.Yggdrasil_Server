#include "pch.h"
#include "Exception_Handler.h"
#include "CLogMgr.h"
#include "CProtocolMgr.h"
#include "CLoginMgr.h"
#include "CLobbyMgr.h"
#include "CRoomMgr.h"
#include "CGameMgr.h"
#include "CIocp.h"

Exception_Handler* volatile Exception_Handler::m_instance = nullptr;
CLock* Exception_Handler::m_lock = new CLock();

DWORD Exception_Handler::_initalize(LPCTSTR dump_file_name, const MINIDUMP_TYPE dump_type)
{
	DWORD error = ERROR_SUCCESS;
	do
	{
		if (nullptr == dump_file_name)
		{
			error = ERROR_INVALID_PARAMETER;
			break;
		}
		_dump_file_name.assign(dump_file_name);
		_dump_type = dump_type;
	} while (false);

	return error;
}

DWORD Exception_Handler::run()
{
	_prev_filter = ::SetUnhandledExceptionFilter(Exception_Callback);
	return ERROR_SUCCESS;
}

LONG __stdcall Exception_Handler::Exception_Callback(_EXCEPTION_POINTERS* exceptioninfo)
{
	do
	{
		if (nullptr == exceptioninfo)
		{
			break;
		}
		SYSTEMTIME st = { 0 };
		::GetLocalTime(&st);

		std::wstring dump_file_name;
		dump_file_name.assign(Exception_Handler::Instance()->_dump_file_name);

		HANDLE dump_file_handle = ::CreateFile(
			dump_file_name.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		if (INVALID_HANDLE_VALUE == dump_file_handle)
		{
			break;
		}
		MINIDUMP_EXCEPTION_INFORMATION ex_info = { 0 };
		ex_info.ThreadId = ::GetCurrentThreadId();
		ex_info.ExceptionPointers = exceptioninfo;
		ex_info.ClientPointers = FALSE;

		if (FALSE == ::MiniDumpWriteDump(
			::GetCurrentProcess(),
			::GetCurrentProcessId(),
			dump_file_handle,
			Exception_Handler::Instance()->_dump_type,
			&ex_info,
			nullptr, nullptr
		))
		{
			break;
		}

	} while (false);

	MEMORYSTATUS memoryinfo;
	GlobalMemoryStatus(&memoryinfo);

	t_ThreadInfo* curthread = CIocp::g_threadinfo[::GetCurrentThreadId()];
	wstring iotype = IOTYPEtoString(curthread->io_type);
	wstring state = ESTATEtoString(curthread->cur_state);
	wstring main, sub, detail;
	PROTOCOLtoString(curthread->protocol, curthread->cur_state,&main, &sub, &detail);
	CLogMgr::GetInst()->
		FileWriteLog(_T("\n\nmemory info\ndwMemoryLoad: %lu\ndwTotalPhys: %lu\ndwAvailPhys: %lu\ndwTotalPageFile: %lu\ndwAvilPageFile: %lu\ndwTotalVirtual: %lu\ndwAvailVirtual: %lu\n\n\n\nthread info\nIO_TYPE: %s\nE_STATE: %s\nMAINPROTOCOL:%s\nSUBPROTOCOL:%s\nDETAILPROTOCOL:%s\n")
			, memoryinfo.dwMemoryLoad/(1024*2), memoryinfo.dwTotalPhys / (1024 * 1024), memoryinfo.dwAvailPhys / (1024 * 1024), memoryinfo.dwTotalPageFile / (1024 * 1024), memoryinfo.dwAvailPageFile / (1024 * 1024), memoryinfo.dwTotalVirtual / (1024 * 1024), memoryinfo.dwAvailVirtual / (1024 * 1024),iotype.c_str(),state.c_str(),main.c_str(),sub.c_str(),detail.c_str());

	return (Exception_Handler::Instance()->_prev_filter) ? Exception_Handler::Instance()->_prev_filter(exceptioninfo) : EXCEPTION_EXECUTE_HANDLER;
}

wstring IOTYPEtoString(IO_TYPE _type)
{
	wstring type;
	switch (_type)
	{
	case IO_TYPE::RECV:
		type = _T("RECV");
		break;
	case IO_TYPE::SEND:
		type = _T("SEND");
		break;
	case IO_TYPE::ACCEPT:
		type = _T("ACCEPT");
		break;
	case IO_TYPE::DISCONNECT:
		type = _T("DISCONNECT");
		break;
	}
	return type;
}

wstring ESTATEtoString(E_STATE _state)
{
	wstring type;
	switch (_state)
	{
	case E_STATE::LOGIN:
		type = _T("LOGIN");
		break;
	case E_STATE::LOBBY:
		type = _T("LOBBY");
		break;
	case E_STATE::ROOM:
		type = _T("ROOM");
		break;
	case E_STATE::GAME:
		type = _T("GAME");
		break;
	}
	return type;
}

void PROTOCOLtoString(unsigned long& original, E_STATE& curstate, wstring* outProtocol_1, wstring* outProtocol_2, wstring* outProtocol_3)
{
	unsigned long main = CProtocolMgr::GetInst()->GetMainProtocol(original);
	switch (static_cast<MAINPROTOCOL>(main))
	{
	case MAINPROTOCOL::LOGIN:
		*outProtocol_1 = _T("LOGIN");
		break;
	case MAINPROTOCOL::LOBBY:
		*outProtocol_1 = _T("LOBBY");
		break;
	case MAINPROTOCOL::ROOM:
		*outProtocol_1 = _T("ROOM");
		break;
	case MAINPROTOCOL::GAME:
		*outProtocol_1 = _T("GAME");
		break;
	case MAINPROTOCOL::INIT:
		*outProtocol_1 = _T("INIT");
		break;
	case MAINPROTOCOL::TEST:
		*outProtocol_1 = _T("TEST");
		break;
	}
	unsigned long sub = CProtocolMgr::GetInst()->GetSubProtocol(original);
	unsigned long detail = CProtocolMgr::GetInst()->GetDetailProtocol(original);
	switch (curstate)
	{
	case E_STATE::LOGIN:
		switch (static_cast<CLoginMgr::SUBPROTOCOL>(sub))
		{
		case CLoginMgr::SUBPROTOCOL::LoginInfo:
			*outProtocol_2 = _T("LoginInfo");
			break;
		case CLoginMgr::SUBPROTOCOL::LoginResult:
			*outProtocol_2 = _T("LoginResult");
			break;
		case CLoginMgr::SUBPROTOCOL::JoinInfo:
			*outProtocol_2 = _T("JoinInfo");
			break;
		case CLoginMgr::SUBPROTOCOL::JoinResult:
			*outProtocol_2 = _T("JoinResult");
			break;
		case CLoginMgr::SUBPROTOCOL::LogoutInfo:
			*outProtocol_2 = _T("LogoutInfo");
			break;
		case CLoginMgr::SUBPROTOCOL::LogoutResult:
			*outProtocol_2 = _T("LogoutResult");
			break;
		}
		*outProtocol_3 = _T("DETAILPROTOCOL ¾øÀ½");
		break;
	case E_STATE::LOBBY:
		switch (static_cast<CLobbyMgr::SUBPROTOCOL>(sub))
		{
		case CLobbyMgr::SUBPROTOCOL::BackPage:
			*outProtocol_2 = _T("BackPage");
			break;
		case CLobbyMgr::SUBPROTOCOL::Init:
			*outProtocol_2 = _T("Init");
			break;
		case CLobbyMgr::SUBPROTOCOL::Multi:
			*outProtocol_2 = _T("Multi");
			break;
		case CLobbyMgr::SUBPROTOCOL::Single:
			*outProtocol_2 = _T("Single");
			break;
		}
		switch (detail)
		{
		case static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::LobbyEnter):
			*outProtocol_3 = _T("LobbyEnter");
			break;
		case static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::LobbyResult):
			*outProtocol_3 = _T("LobbyResult");
			break;
		case static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::RoomlistResult):
			*outProtocol_3 = _T("RoomlistResult");
			break;
		case static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::CreateRoom):
			*outProtocol_3 = _T("CreateRoom");
			break;
		case static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::CreateRoomResult):
			*outProtocol_3 = _T("CreateRoomResult");
			break;
		case (static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::RoomlistUpdate) | static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::PageRoom)):
			*outProtocol_3 = _T("RoomlistUpdate _ PageRoom");
			break;
		case (static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::RoomlistUpdate) | static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::AllRoom)):
			*outProtocol_3 = _T("RoomlistUpdate _ AllRoom");
			break;
		case (static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::AllMsg)):
			*outProtocol_3 = _T("ChatSend _ AllMsg");
			break;
		case (static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::NoticeMsg)):
			*outProtocol_3 = _T("ChatSend _ NoticeMsg");
			break;
		case (static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::ChatRecv) | static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::AllMsg)):
			*outProtocol_3 = _T("ChatRecv _ AllMsg");
			break;
		case (static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::ChatRecv) | static_cast<const unsigned long>(CLobbyMgr::DETAILPROTOCOL::NoticeMsg)):
			*outProtocol_3 = _T("ChatRecv _ NoticeMsg");
			break;
		}
		break;
	case E_STATE::ROOM:
		switch (static_cast<CRoomMgr::SUBPROTOCOL>(sub))
		{
		case CRoomMgr::SUBPROTOCOL::BackPage:
			*outProtocol_2 = _T("BackPage");
			break;
		case CRoomMgr::SUBPROTOCOL::Init:
			*outProtocol_2 = _T("Init");
			break;
		case CRoomMgr::SUBPROTOCOL::Multi:
			*outProtocol_2 = _T("Multi");
			break;
		case CRoomMgr::SUBPROTOCOL::Single:
			*outProtocol_2 = _T("Single");
			break;
		}
		switch (detail)
		{
		case static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::CharacterResult):
			*outProtocol_3 = _T("CharacterResult");
			break;
		case static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::CharacterSelect):
			*outProtocol_3 = _T("CharacterSelect");
			break;
		case static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::RoomEnter):
			*outProtocol_3 = _T("RoomEnter");
			break;
		case static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::RoomResult):
			*outProtocol_3 = _T("RoomResult");
			break;
		case static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::ReadyResult):
			*outProtocol_3 = _T("ReadyResult");
			break;
		case  static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::ReadySelect) | static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::HostReady) :
			*outProtocol_3 = _T("ReadySelect_ HostReady");
		   break;
		case  static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::ReadySelect) | static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::NomalReady) :
			*outProtocol_3 = _T("ReadySelect_ NomalReady");
		   break;
		case static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::ChatSend) | static_cast<const unsigned long>(CRoomMgr::DETAILPROTOCOL::AllMsg) :
			*outProtocol_3 = _T("ChatSend_ AllMsg");
			break;
		}
		break;
	case E_STATE::GAME:
		switch (static_cast<CGameMgr::SUBPROTOCOL>(sub))
		{
		case CGameMgr::SUBPROTOCOL::Object:
			*outProtocol_2 = _T("Object");
			break;
		case CGameMgr::SUBPROTOCOL::SECTOR:
			*outProtocol_2 = _T("Sector");
			break;
		}
		switch (static_cast<CGameMgr::DETAILPROTOCOL>(detail))
		{
		case CGameMgr::DETAILPROTOCOL::Tile:
			*outProtocol_3 = _T("Tile");
			break;
		case CGameMgr::DETAILPROTOCOL::Player:
			*outProtocol_3 = _T("Player");
			break;
		case CGameMgr::DETAILPROTOCOL::Boss:
			*outProtocol_3 = _T("Boss");
			break;
		case CGameMgr::DETAILPROTOCOL::Item:
			*outProtocol_3 = _T("Item");
			break;
		}
		break;
	}
}


