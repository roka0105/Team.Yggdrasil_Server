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
		ZeroMemory(password, STRINGSIZE);
		host = nullptr;
		mode = -1;
	}
	t_RoomInfo(unsigned int _id,ENetObjectType _type, TCHAR* _name,TCHAR* _password, CSession* _host)
	{
		id = _id;
		type = _type;
		ZeroMemory(name, STRINGSIZE);
		ZeroMemory(password, STRINGSIZE);
		_tcscpy(name, _name);
		_tcscpy(password, _password);
		host = _host;
		mode = -1;
	}
	unsigned int id;
	ENetObjectType type;
	TCHAR name[STRINGSIZE];
	TCHAR password[STRINGSIZE];
	list<CSession*> sessions;
	CSession* host;
	unsigned int mode;
};
class CRoomMgr
{
public:
	enum class ERRTYPE
	{
		NONE,
		ERR_MAXENTER,
		ERR_PW,
		ERR_ROOMINDEX,
	};
	enum class SUBPROTOCOL
	{
		NONE = -1,
		RoomEnter,
		RoomResult,

		MAX
	};
	enum class DETAILPROTOCOL
	{
		NONE = -1,
	
		MAX
	};
	static CRoomMgr* GetInst();
	static void Create();
	static void Destroy();
	void Init();
	void End();

    void SendInit(CSession* _session);
	bool EnterRoomProcess(CSession* _session);
	//방 추가
	void AddRoom(CSession* _host);
	//방 삭제
	void RemoveRoom(unsigned int _id);
	//get 해당 아이디 값을 가진 방 정보 전송 이거는 아직 어떤식으로 쓸지 생각이 안나서 주석.
	//void SendRoom(CSession* _session,unsigned int _id);
	//get 모든 방 정보 전송 이것도 잘 안사용할듯..
	//void SendRoom(CSession* _session);
	//get 현재 페이지 방 정보 전송
	void SendRoom(unsigned int page,CSession* _session);
	bool PageCheck(unsigned int page);
	ERRTYPE EnterCheck(int _roomindex,t_RoomInfo** _roominfo, const TCHAR* _pw);
private:
	CRoomMgr();
	~CRoomMgr();
    void Packing(unsigned long _protocol, int _roomcount, CSession* _session);
	//방 1개 정보 packing
	void Packing(unsigned long _protocol,t_RoomInfo* _room, CSession* _session);
	//방 묶음 정보 packing
	void Packing(unsigned long _protocol,bool result,int page,list<t_RoomInfo*> _rooms, CSession* _session);
	//방 입장 결과 packing
	void Packing(unsigned long _protocol,int result,t_RoomInfo* _room, CSession* _session);
	//입장할 방 번호 unpacking
	void UnPacking(byte* _recvdata, int& _roomindex,TCHAR* _pw);
	//생성할 방 정보 unpacking
	void UnPacking(byte* _recvdata,TCHAR* _name,TCHAR* _pw);
private:
	static CRoomMgr* instance;
	const unsigned int m_enter_limit = 3;
	const unsigned int m_packet_room_count = 77;
	unsigned int m_max_page = 0;
	unsigned int m_rooms_count = 0;               //현재 방 갯수 카운팅.
	const unsigned int m_page_room_count = 10;
	CLock* m_lock;
	map<unsigned int, list<t_RoomInfo*>> m_rooms;
};

