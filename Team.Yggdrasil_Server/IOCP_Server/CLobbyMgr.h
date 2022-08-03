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
		Multi,
		Single,
		BackPage,
		MAX
	};
	enum class DETAILPROTOCOL
	{
		NONE = -1,
		//========����=========
		LobbyEnter=1,
		LobbyResult=2,
		CreateRoom=4,
		CreateRoomResult=8,
		ChatSend=16,
		ChatRecv=32,
		RoomlistUpdate=64,
		RoomlistResult=128,
		//========����=========post
		NoticeMsg = 256,//���� �޼��� (��ڰ� ����)
		AllMsg = 512,//��ü �޼��� (�Ϲ� �������� ���)
		AllRoom = 1024,
		PageRoom = 2048,
		MAX
	};
	static CLobbyMgr* GetInst();
	static void Create();
	static void Destroy();
	void Init();
	void End();

	void LobbyProcess(CSession* _session);
	void EnterRoomProcess(CSession* _session);
	void BackPageProcess(CSession* _session);

	void LobbyEnterFunc(CSession* _session);
	//void AllRoomFunc(CSession* _session);
	void CreateRoomFunc(CSession* _session);
	void PageRoomFunc(CSession* _session);
	void ChattingFunc(CSession* _session);

	void AddLobbySession(CSession* _session);
private:
	CLobbyMgr();
	~CLobbyMgr();

	void UnPacking(byte* _recvdata, TCHAR* msg);
	void Packing(unsigned long _protocol,bool result, TCHAR* msg,CSession* _session);
	void Packing(unsigned long _protocol, bool result, CSession* _session);
private :
	CLock* m_lock;
	static CLobbyMgr* instance;
	list<CSession*> m_lobby_session_list; //�κ� ���� �ִ� ���� ����.
};

//multi ���� ���۵Ǿ�� �� ���� 
//1.�� ����Ʈ(���ȣ=ID,�� �̸�, �������,�� ���� �ο���,���ο�����) �̹����� �׳� Ŭ�󿡼� load
