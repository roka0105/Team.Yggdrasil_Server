#include "pch.h"
#include "CLoginMgr.h"
#include "CSession.h"
#include "CMainMgr.h"
#include "CDBMgr.h"
#include "CLogMgr.h"
#include "CProtocolMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "CLobbyMgr.h"
#include "CRoomMgr.h"



CLoginMgr* CLoginMgr::m_instance = nullptr;

CLoginMgr* CLoginMgr::GetInst()
{
	return m_instance;
}

void CLoginMgr::Create()
{
	if (m_instance == nullptr)
		m_instance = new CLoginMgr();
}

void CLoginMgr::Destroy()
{
	delete m_instance;
}

CLoginMgr::CLoginMgr()
{

}

CLoginMgr::~CLoginMgr()
{

}

void CLoginMgr::Init()
{
	m_lock = new CLock();
}

void CLoginMgr::End()
{
	//로그인 정보 모두 로그아웃 시키기.
	m_loginlist.clear();
	m_joinlist.clear();
	delete m_lock;
}


/* 메뉴 로그인버튼 회원가입버튼 */
void CLoginMgr::LoginProcess(CSession* _ptr)
{
	unsigned long protocol = 0;
	unsigned long subprotocol = 0;

	//1. 클라에서 로그인 정보 받아옴.
	//2. 받아온 정보 UnPackPacket
	_ptr->UnPacking(protocol);
	subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	switch ((SUBPROTOCOL)subprotocol)
	{
	case SUBPROTOCOL::LoginInfo:
		LoginFunc(_ptr);
		break;
	case SUBPROTOCOL::JoinInfo:
		JoinFunc(_ptr);
		break;
	case SUBPROTOCOL::LogoutInfo:
		LogOutFunc(_ptr);
		break;
	}

}
void CLoginMgr::LoginFunc(CSession* _ptr)
{
	CLockGuard<CLock> lock(m_lock);
	TCHAR ID[BUFSIZE], PW[BUFSIZE], NICK[BUFSIZE], temp[BUFSIZE], msg[BUFSIZE];
	byte buf[BUFSIZE], sendbuf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	ZeroMemory(ID, BUFSIZE);
	ZeroMemory(PW, BUFSIZE);
	ZeroMemory(NICK, BUFSIZE);
	ZeroMemory(sendbuf, BUFSIZE);
	ZeroMemory(temp, BUFSIZE);
	ZeroMemory(msg, BUFSIZE);
	int size = 0;
	int p;

	bool result = false;
	unsigned long protocol = 0;
	unsigned long subprotocol = 0;
	_ptr->UnPacking(buf);
	UnPacking(buf, ID, PW);
	ZeroMemory(buf, BUFSIZE);
	//3. 받아온 정보로 로그인 가능 여부 체크
	if (LoginCheck(ID, PW, NICK))//로그인 성공
	{
		_ptr->SetUserInfo(ID, PW, NICK, true);
		m_loginlist.push_back(_ptr->GetUserInfo());
		_stprintf(msg, _T("%s님이 로그인 성공! 환영합니다!"), NICK);
		result = true;
#ifdef _DEBUG
		CLogMgr::GetInst()->WriteLog(_T("데이터: 로그인 성공! %s, %s, %s\n"), ID, PW, NICK);
#endif	
	}
	else//로그인 실패
	{
#ifdef _DEBUG
		CLogMgr::GetInst()->WriteLog(_T("데이터: 로그인 실패! %s, %s, %s\n"), ID, PW, NICK);
#endif	
		_tcscpy(msg, _T("로그인 실패!"));
	}
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, (unsigned long)MAINPROTOCOL::LOGIN);
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, (unsigned long)SUBPROTOCOL::LoginResult);
	//4. 로그인 성공 여부를 패킹 sendlist가 보내고 있는 중인 데이터가 없다면 바로 클라에 전송.
	Packing(protocol, result, msg, _ptr);

}
void CLoginMgr::JoinFunc(CSession* _ptr)
{
	CLockGuard<CLock> lock(m_lock);
	//1. 클라가 보낸 회원가입 정보를 받으면, 
	//2. 등록된 정보인지 확인하여 결과에 따라 packet을 조립하여 Send한다.

	TCHAR ID[BUFSIZE], PW[BUFSIZE], NICK[BUFSIZE], msg[BUFSIZE];
	byte buf[BUFSIZE], sendbuf[BUFSIZE];
	ZeroMemory(buf, BUFSIZE);
	ZeroMemory(ID, BUFSIZE);
	ZeroMemory(PW, BUFSIZE);
	ZeroMemory(NICK, BUFSIZE);
	ZeroMemory(sendbuf, BUFSIZE);
	int size = 0;
	int p;

	TCHAR temp[BUFSIZE];
	bool result = false;
	unsigned long protocol = 0;
	unsigned long subprotocol = 0;


	_ptr->UnPacking(buf);

	UnPacking(buf, ID, PW, NICK);
	ZeroMemory(buf, BUFSIZE);
	if (joinCheck(msg, ID, NICK)) // 회원가입 성공
	{
		t_UserInfo* user = new t_UserInfo(ID, PW, NICK);
		m_joinlist.push_back(user);
		CDBMgr::GetInst()->InsertJointbl(user);
		_stprintf(msg, _T("%s님 회원가입에 성공하였습니다!"), NICK);
		ZeroMemory(temp, BUFSIZE);
		result = true;
#ifdef _DEBUG
		CLogMgr::GetInst()->FileWriteLog(_T("데이터: 회원가입 성공! %s, %s, %s\n"), ID, PW, NICK);
		CLogMgr::GetInst()->WriteLog(_T("데이터: 로그인 성공! %s, %s, %s\n"), ID, PW, NICK);
		printf("\n=====\n");
		CLogMgr::GetInst()->FileReadLogAll();
		//CLogMgr::GetInst()->WriteLog("데이터: 회원가입 성공! %s, %s, %s\n", ID, PW, NICK);
		TCHAR temp2[BUFSIZE];
		ZeroMemory(temp2, BUFSIZE);
		_stprintf(temp2, _T("데이터: 회원가입 성공! %s, %s, %s\n"), ID, PW, NICK);
		CDBMgr::GetInst()->InsertJoinLog(temp2);
#else
		_stprintf(temp, _T("회원가입 성공 %s, %s , %s\n"), ID, PW, NICK);
		CDBMgr::GetInst()->InsertJoinLog(temp);
#endif	
	}
	else // 회원가입 실패
	{
#ifdef _DEBUG
		CLogMgr::GetInst()->WriteLog(_T("데이터: 회원가입 실패! %s, %s, %s\n"), ID, PW, NICK);
#endif	
		_tcscpy(msg, _T("회원가입 실패!"));
	}


	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::LOGIN));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::JoinResult));
	Packing(protocol, result, msg, _ptr);
}

void CLoginMgr::LogOutFunc(CSession* _ptr)
{
	CLockGuard<CLock> lock(m_lock);
	byte sendbuf[BUFSIZE];
	ZeroMemory(sendbuf, BUFSIZE);

	unsigned long protocol = 0;
	unsigned long subprotocol = 0;

	m_loginlist.remove(_ptr->GetUserInfo());
	_ptr->SetUserInfo(NULL, NULL, NULL, false);

	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::LOGIN));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(SUBPROTOCOL::LogoutResult));
	Packing(protocol, _T("로그아웃에 성공하였습니다!"), _ptr);
}

void CLoginMgr::EnterLobbyProcess(CSession* _ptr)
{
	CLockGuard<CLock> lock(m_lock);

	unsigned long protocol = 0; 
	unsigned long subprotocol = 0;


	_ptr->UnPacking(protocol);
	subprotocol = CProtocolMgr::GetInst()->GetSubProtocol(protocol);
	switch (static_cast<CLobbyMgr::SUBPROTOCOL>(subprotocol))
	{
	case CLobbyMgr::SUBPROTOCOL::Multi:
		CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::SUBPROTOCOL::Multi));
		// 방 리스트 정보 보낸다.
		CRoomMgr::GetInst()->SendRoom(0, _ptr);
		//로비에 들어와있는 유저 리스트에 유저정보 추가한다.
		CLobbyMgr::GetInst()->AddLobbySession(_ptr);
		break;
	case CLobbyMgr::SUBPROTOCOL::Single:
		CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::SUBPROTOCOL::Single));
		break;
	}

	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::LOBBY));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(CLobbyMgr::DETAILPROTOCOL::LobbyResult));

	_ptr->Packing(protocol, nullptr, 0);
}

BOOL CLoginMgr::LoginCheck(TCHAR* _id, TCHAR* _pw, TCHAR* _nick)
{
	//1.회원정보와 일치하는 지 체크
	if (m_joinlist.size() > 0)
	{
		for (t_UserInfo* userinfo : m_joinlist)
		{
			if (!_tcscmp(userinfo->id, _id) && !_tcscmp(userinfo->pw, _pw))
			{
				if (m_loginlist.size() == 0)
				{
					_tcscpy(_nick, userinfo->nickname);
					return true;
				}
				//2.로그인중인 list와 일치하지 않는지 체크
				for (t_UserInfo* loginfo : m_loginlist)
				{
					//이미 로그인 중인 경우
					if (!memcmp(loginfo, userinfo, sizeof(t_UserInfo)))
					{
						return false;
					}
					else//로그인 성공
					{
						_tcscpy(_nick, userinfo->nickname);
						return true;
					}
				}

			}
		}
	}
	return false;
}

BOOL CLoginMgr::joinCheck(TCHAR* _msg, TCHAR* _id, TCHAR* _nick)
{
	for (t_UserInfo* tuserinfo : m_joinlist)
	{
		// 등록된 ID를 찾은 경우
		if (!_tcscmp(tuserinfo->id, _id))
		{
			_tcscpy(_msg, _T("이미 등록된 ID입니다.\n"));
			return false;
		}
		// 등록된 NICK을 찾은 경우
		if (!_tcscmp(tuserinfo->nickname, _nick))
		{
			_tcscpy(_msg, _T("이미 존재하는 NICKNAME입니다.\n"));
			return false;
		}
	}
	return true;
}

unsigned long CLoginMgr::GetProtocol(byte* _recvbuf)
{
	unsigned long protocol;
	memcpy(&protocol, _recvbuf, sizeof(unsigned long));
	return protocol;
}
void CLoginMgr::Packing(unsigned long _protocol, const TCHAR* _id, const TCHAR* _pw, CSession* _ptr)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	int strsize = _tcslen(_id) * CODESIZE;
	unsigned long protocol = _protocol;

	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _id, strsize);
	size += strsize;
	ptr += strsize;

	strsize = _tcslen(_pw) * CODESIZE;
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _pw, strsize);
	size += strsize;
	ptr += strsize;

	_ptr->Packing(protocol, _buf, size);
}
void CLoginMgr::Packing(unsigned long _protocol, const TCHAR* _str, CSession* _ptr)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	int strsize = _tcslen(_str) * CODESIZE;
	unsigned long protocol = _protocol;

	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _str, strsize);
	size += strsize;
	ptr += strsize;

	_ptr->Packing(protocol, _buf, size);
}
void CLoginMgr::Packing(unsigned long _protocol, bool _flag, const TCHAR* _str, CSession* _ptr)
{
	byte _buf[BUFSIZE];
	ZeroMemory(_buf, BUFSIZE);
	byte* ptr = _buf;
	int size = 0;
	int strsize = _tcslen(_str) * CODESIZE;
	unsigned long protocol = _protocol;

	memcpy(ptr, &_flag, sizeof(bool));
	size += sizeof(bool);
	ptr += sizeof(bool);

	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _str, strsize);
	size += strsize;
	ptr += strsize;

	_ptr->Packing(protocol, _buf, size);
}
void CLoginMgr::UnPacking(const byte* _buf, TCHAR* _id, TCHAR* _pw)
{
	const byte* ptr = _buf;
	int strsize = 0;

	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_id, ptr, strsize * CODESIZE);
	ptr += strsize * CODESIZE;

	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_pw, ptr, strsize * CODESIZE);
	ptr += strsize * CODESIZE;
}

void CLoginMgr::UnPacking(const byte* _buf, TCHAR* _id, TCHAR* _pw, TCHAR* _nickname)
{
	const byte* ptr = _buf;
	int strsize = 0;

	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_id, ptr, strsize * CODESIZE);
	ptr += strsize * CODESIZE;

	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_pw, ptr, strsize * CODESIZE);
	ptr += strsize * CODESIZE;

	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_nickname, ptr, strsize * CODESIZE);
	ptr += strsize * CODESIZE;
}

BOOL CLoginMgr::SearchFile(const TCHAR* filename)
{
	CLockGuard<CLock> lock(m_lock);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = FindFirstFile(filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		FindClose(hFindFile);
		return TRUE;
	}
}

bool CLoginMgr::FileDataLoad()
{
	CLockGuard<CLock> lock(m_lock);
	if (!SearchFile(_T("UserInfo.info")))
	{
		FILE* fp = fopen("UserInfo.info", "wb");
		fclose(fp);
		return true;
	}

	FILE* fp = fopen("UserInfo.info", "rb");
	if (fp == NULL)
	{
		return false;
	}

	t_UserInfo info;
	memset(&info, 0, sizeof(t_UserInfo));

	while (1)
	{
		fread(&info, sizeof(t_UserInfo), 1, fp);
		if (feof(fp))
		{
			break;
		}
		t_UserInfo* ptr = new t_UserInfo;
		memcpy(ptr, &info, sizeof(t_UserInfo));
		m_joinlist.push_back(ptr);
	}

	fclose(fp);
	return true;
}

bool CLoginMgr::FileDataAdd(t_UserInfo* _info)
{
	CLockGuard<CLock> lock(m_lock);
	FILE* fp = fopen("UserInfo.info", "ab");
	if (fp == NULL)
	{
		return false;
	}

	int retval = fwrite(_info, 1, sizeof(t_UserInfo), fp);

	if (retval != sizeof(t_UserInfo))
	{
		return false;
	}

	fclose(fp);

	return true;
}

