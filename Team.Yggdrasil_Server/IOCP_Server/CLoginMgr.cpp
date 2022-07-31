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
	//�α��� ���� ��� �α׾ƿ� ��Ű��.
	m_loginlist.clear();
	m_joinlist.clear();
	delete m_lock;
}


/* �޴� �α��ι�ư ȸ�����Թ�ư */
void CLoginMgr::LoginProcess(CSession* _ptr)
{
	unsigned long protocol = 0;
	unsigned long subprotocol = 0;

	//1. Ŭ�󿡼� �α��� ���� �޾ƿ�.
	//2. �޾ƿ� ���� UnPackPacket
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
	//3. �޾ƿ� ������ �α��� ���� ���� üũ
	if (LoginCheck(ID, PW, NICK))//�α��� ����
	{
		_ptr->SetUserInfo(ID, PW, NICK, true);
		m_loginlist.push_back(_ptr->GetUserInfo());
		_stprintf(msg, _T("%s���� �α��� ����! ȯ���մϴ�!"), NICK);
		result = true;
#ifdef _DEBUG
		CLogMgr::GetInst()->WriteLog(_T("������: �α��� ����! %s, %s, %s\n"), ID, PW, NICK);
#endif	
	}
	else//�α��� ����
	{
#ifdef _DEBUG
		CLogMgr::GetInst()->WriteLog(_T("������: �α��� ����! %s, %s, %s\n"), ID, PW, NICK);
#endif	
		_tcscpy(msg, _T("�α��� ����!"));
	}
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, (unsigned long)MAINPROTOCOL::LOGIN);
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, (unsigned long)SUBPROTOCOL::LoginResult);
	//4. �α��� ���� ���θ� ��ŷ sendlist�� ������ �ִ� ���� �����Ͱ� ���ٸ� �ٷ� Ŭ�� ����.
	Packing(protocol, result, msg, _ptr);

}
void CLoginMgr::JoinFunc(CSession* _ptr)
{
	CLockGuard<CLock> lock(m_lock);
	//1. Ŭ�� ���� ȸ������ ������ ������, 
	//2. ��ϵ� �������� Ȯ���Ͽ� ����� ���� packet�� �����Ͽ� Send�Ѵ�.

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
	if (joinCheck(msg, ID, NICK)) // ȸ������ ����
	{
		t_UserInfo* user = new t_UserInfo(ID, PW, NICK);
		m_joinlist.push_back(user);
		CDBMgr::GetInst()->InsertJointbl(user);
		_stprintf(msg, _T("%s�� ȸ�����Կ� �����Ͽ����ϴ�!"), NICK);
		ZeroMemory(temp, BUFSIZE);
		result = true;
#ifdef _DEBUG
		CLogMgr::GetInst()->FileWriteLog(_T("������: ȸ������ ����! %s, %s, %s\n"), ID, PW, NICK);
		CLogMgr::GetInst()->WriteLog(_T("������: �α��� ����! %s, %s, %s\n"), ID, PW, NICK);
		printf("\n=====\n");
		CLogMgr::GetInst()->FileReadLogAll();
		//CLogMgr::GetInst()->WriteLog("������: ȸ������ ����! %s, %s, %s\n", ID, PW, NICK);
		TCHAR temp2[BUFSIZE];
		ZeroMemory(temp2, BUFSIZE);
		_stprintf(temp2, _T("������: ȸ������ ����! %s, %s, %s\n"), ID, PW, NICK);
		CDBMgr::GetInst()->InsertJoinLog(temp2);
#else
		_stprintf(temp, _T("ȸ������ ���� %s, %s , %s\n"), ID, PW, NICK);
		CDBMgr::GetInst()->InsertJoinLog(temp);
#endif	
	}
	else // ȸ������ ����
	{
#ifdef _DEBUG
		CLogMgr::GetInst()->WriteLog(_T("������: ȸ������ ����! %s, %s, %s\n"), ID, PW, NICK);
#endif	
		_tcscpy(msg, _T("ȸ������ ����!"));
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
	Packing(protocol, _T("�α׾ƿ��� �����Ͽ����ϴ�!"), _ptr);
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
		// �� ����Ʈ ���� ������.
		CRoomMgr::GetInst()->SendRoom(0, _ptr);
		//�κ� �����ִ� ���� ����Ʈ�� �������� �߰��Ѵ�.
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
	//1.ȸ�������� ��ġ�ϴ� �� üũ
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
				//2.�α������� list�� ��ġ���� �ʴ��� üũ
				for (t_UserInfo* loginfo : m_loginlist)
				{
					//�̹� �α��� ���� ���
					if (!memcmp(loginfo, userinfo, sizeof(t_UserInfo)))
					{
						return false;
					}
					else//�α��� ����
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
		// ��ϵ� ID�� ã�� ���
		if (!_tcscmp(tuserinfo->id, _id))
		{
			_tcscpy(_msg, _T("�̹� ��ϵ� ID�Դϴ�.\n"));
			return false;
		}
		// ��ϵ� NICK�� ã�� ���
		if (!_tcscmp(tuserinfo->nickname, _nick))
		{
			_tcscpy(_msg, _T("�̹� �����ϴ� NICKNAME�Դϴ�.\n"));
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

