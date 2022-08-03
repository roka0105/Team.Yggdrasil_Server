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
		//========상위=========
		LobbyEnter=1,
		LobbyResult=2,
		CreateRoom=4,
		CreateRoomResult=8,
		ChatSend=16,
		ChatRecv=32,
		RoomlistUpdate=64,
		RoomlistResult=128,
		//========하위=========post
		NoticeMsg = 256,//공지 메세지 (운영자가 전송)
		AllMsg = 512,//전체 메세지 (일반 유저들이 사용)
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
	list<CSession*> m_lobby_session_list; //로비에 들어와 있는 유저 정보.
};

//multi 들어갈때 전송되어야 할 정보 
//1.방 리스트(방번호=ID,방 이름, 모드정보,방 참여 인원수,방인원제한) 이미지는 그냥 클라에서 load
