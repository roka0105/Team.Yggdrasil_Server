#pragma once
#include "CLogMgr.h"
class CSession;
class CLock;

struct t_RoomInfo
{
	t_RoomInfo()
	{
		id = -1;
		ZeroMemory(name, STRINGSIZE);
		host = nullptr;
		mode = -1;
	}
	t_RoomInfo(unsigned int _id, TCHAR* _name, CSession* _host, unsigned int _mode)
	{
		id = _id;
		ZeroMemory(name, STRINGSIZE);
		_tcscpy(name, _name);
		host = _host;
		mode = _mode;
	}
	unsigned int id;
	TCHAR name[STRINGSIZE];
	list<CSession*> sessions;
	CSession* host;
	unsigned int mode;
};
class CRoomMgr
{
public:
	static CRoomMgr* GetInst();
	static void Create();
	static void Destroy();
	//방 추가
	void AddRoom(TCHAR* _name,CSession* _host,unsigned int _mode);
	//방 삭제
	void RemoveRoom(unsigned int _id);
	//get 해당 아이디 값을 가진 방 정보 전송 이거는 아직 어떤식으로 쓸지 생각이 안나서 주석.
	//void SendRoom(CSession* _session,unsigned int _id);
	//get 모든 방 정보 전송 이것도 잘 안사용할듯..
	void SendRoom(CSession* _session);
	//get 현재 페이지 방 정보 전송
	void SendRoom(unsigned int page,CSession* _session);
private:
	CRoomMgr();
	~CRoomMgr();
	//방 1개 정보 packing
	void Packing(unsigned long _protocol,t_RoomInfo* _room, CSession* _session);
	//방 묶음 정보 packing
	void Packing(unsigned long _protocol,list<t_RoomInfo*> _rooms, CSession* _session);
	//생성할 방 정보 unpacking
	void UnPacking(TCHAR* _name,unsigned int _mode);
private:
	static CRoomMgr* instance;
	const unsigned int m_enter_limit = 3;
	const unsigned int m_packet_room_count = 77;
	unsigned int m_max_page = 0;
	const unsigned int m_page_room_count = 10;
	CLock* m_lock;
	list<t_RoomInfo*> m_rooms;
};

