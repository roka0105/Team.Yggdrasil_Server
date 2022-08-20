#pragma once
#include "CLogMgr.h"
#include "CSession.h"
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
		NONE_ANOTHER_ENTER,
		ERR_MAXENTER,
		ERR_PW,
		ERR_ROOMINDEX,
		ERR_CHARACTER, //이미 선택 완료한 캐릭터
	};
	enum class SUBPROTOCOL
	{
		NONE = -1,
		Init,
		Multi,
		Single,
		BackPage,
		MAX
	};
	enum class DETAILPROTOCOL
	{
		NONE = -1,
		//========비트 중복 가능========= 3bit
		RoomEnter,
		RoomResult,
		CharacterSelect,
		CharacterResult,
		MapSelect,
		MapResult,
		//========비트 중복 불가능========= 4bit
		ReadySelect = 8,
		ReadyResult = 16,
		ChatSend = 32,
		ChatRecv = 64,
		//========비트 중복 불가능========= 4bit
		HostReady = 128,
		NomalReady = 256,
		NoticeMsg = 512,
		AllMsg = 1024,
		MAX
	};
	static CRoomMgr* GetInst();
	static void Create();
	static void Destroy();
	void Init();
	void End();

    void SendInit(CSession* _session);
	bool EnterRoomProcess(CSession* _session);
	void RoomProcess(CSession* _session);
	void CharacterFunc(CSession* _session);
	void NomalReadyFunc(CSession* _session);
	void HostReadyFunc(CSession* _session);
	//방 추가
	void AddRoom(CSession* _host);
	//방 삭제
	void RemoveRoom(unsigned int _id);
	//get 해당 아이디 값을 가진 방 정보 전송 이거는 아직 어떤식으로 쓸지 생각이 안나서 주석.
	//void SendRoom(CSession* _session,unsigned int _id);
	//get 모든 방 정보 전송 이것도 잘 안사용할듯..
	//void SendRoom(CSession* _session);
	//get 현재 페이지 방 정보 전송
	void SendRoom(bool result, int page,CSession* _session);
	bool PageCheck(int page);
	ERRTYPE EnterCheck(int _roomindex,t_RoomInfo** _roominfo, const TCHAR* _pw);
	ERRTYPE CharacterCheck(const t_RoomInfo* _roominfo, int _type);
	bool AllReadyCheck(t_RoomInfo* _room);
	t_RoomInfo* FindRoom(int _roomindex);
private:
	CRoomMgr();
	~CRoomMgr();
	void Packing(unsigned long _protocol, bool _allready,CSession* _session);
    void Packing(unsigned long _protocol, int _roomcount, CSession* _session);
	//방 1개 정보 packing
	void Packing(unsigned long _protocol,t_RoomInfo* _room, CSession* _session);
	//방 묶음 정보 packing
	void Packing(unsigned long _protocol,bool result,int page,list<t_RoomInfo*> _rooms, CSession* _session);
	//방 입장 결과 packing
	void Packing(unsigned long _protocol,int result,t_RoomInfo* _room, CSession* _session);
	//다른유저의 방 입장정보 packing
	void Packing(unsigned long _protocol, int _result, CPlayer* _enter_player, CSession* _session);
	//캐릭터 선택 결과 packing
	void Packing(unsigned long _protocol, int _result,int _playerid, int _type, CSession* _session);
	//레디 packing
	void Packing(unsigned long _protocol, int _playerid, bool _ready, CSession* _session);
	//입장할 방 번호 unpacking
	void UnPacking(byte* _recvdata, int& _roomindex,TCHAR* _pw);
	//생성할 방 정보 unpacking
	void UnPacking(byte* _recvdata,TCHAR* _name,TCHAR* _pw);
	//캐릭터 선택 정보 unpacking
	void UnPacking(byte* _recvdata, int& _roomindex, int& _type);
	//레디 정보 unpacking
	void UnPacking(byte* _recvdata, int& _roomindex, bool& _ready);
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

