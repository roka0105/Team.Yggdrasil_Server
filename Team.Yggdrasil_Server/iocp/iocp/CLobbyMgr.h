#pragma once
#include "pch.h"
#include "CSession.h"

class CLock;
class CLobbyMgr
{
public :
	enum class SUBPROTOCOL
	{
		NONE,
		LobbyEnter,
		LobbyResult,
		CreateRoom,
		CreateRoomResult,
		ChatSend,
		ChatRecv,
		RoomlistUpdate,
		RoomlistResult,
		MAX
	};
	enum class DETAILPROCOTOL
	{
		NONE = -1,
		Multi = 1,
		Sigle = 2,
		NoticeMsg = 4,//���� �޼��� (��ڰ� ����)
		AllMsg = 8,//��ü �޼��� (�Ϲ� �������� ���)
		AllRoom = 16,
		PageRoom = 32,
		MAX
	};
	static CLobbyMgr* GetInst();
	static void Create();
	static void Destroy();
	void Init();
	void End();
	void MultiModeProcess();
	void SingleModeProcess();
private:
	CLobbyMgr();
	~CLobbyMgr();
private :
	CLock* m_lock;
	static CLobbyMgr* instance;
	list<CSession*> lobby_session_list; //�κ� ���� �ִ� ���� ����.
};

//multi ���� ���۵Ǿ�� �� ���� 
//1.�� ����Ʈ(���ȣ=ID,�� �̸�, �������,�� ���� �ο���,���ο�����) �̹����� �׳� Ŭ�󿡼� load
