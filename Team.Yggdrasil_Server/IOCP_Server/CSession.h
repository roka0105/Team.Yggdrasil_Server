#pragma once
#ifndef _INCLUDE_GUARD_
#define _INCLUDE_GUARD_

#include "CPacket.h"
#include "CState.h"
#include "CLoginState.h"
#include "CLobbyState.h"
#include "CRoomState.h"
#include "CSector.h"
#include "CPlayer.h"
#include "CGameState.h"
#include "HexTile.h"


class CLock;
struct t_UserInfo
{
	t_UserInfo()
	{
		ZeroMemory(id, STRINGSIZE);
		ZeroMemory(pw, STRINGSIZE);
		ZeroMemory(nickname, STRINGSIZE);
		is_login = false;
	}
	t_UserInfo(TCHAR* _id, TCHAR* _pw, TCHAR* _nickname)
	{
		ZeroMemory(id, STRINGSIZE);
		ZeroMemory(pw, STRINGSIZE);
		ZeroMemory(nickname, STRINGSIZE);
		_tcscpy(id, _id);
		_tcscpy(pw, _pw);
		_tcscpy(nickname, _nickname);
		is_login = false;
	}

	TCHAR id[STRINGSIZE];
	TCHAR pw[STRINGSIZE];
	TCHAR nickname[STRINGSIZE];
	bool is_login;
};
struct mygreater
{
	bool operator()(HexTile _t1, HexTile _t2)
	{
		return _t1.GetTime() > _t2.GetTime();
	}
	bool operator()(HexTile* _t1, HexTile* _t2)
	{
		return _t1->GetTime() > _t2->GetTime();
	}
};
struct myless
{
	bool operator()(HexTile _t1, HexTile _t2)
	{
		return _t1.GetTime() < _t2.GetTime();
	}
	bool operator()(HexTile* _t1, HexTile* _t2)
	{
		return _t1->GetTime() < _t2->GetTime();
	}
};
class CSession :public CPacket
{
public:
	CSession(SOCKET _sock) :CPacket(_sock)
	{
		m_loginstate = new CLoginState(this);
		m_lobbystate = new CLobbyState(this);
		m_roomstate = new CRoomState(this);
		m_gamestate = new CGameState(this);
		m_curstate = m_loginstate;
		m_userinfo = new t_UserInfo();

		m_roomid = -1;
		m_gameid = -1;

		m_real_queue = new LRU_Queue<HexTile*,mygreater>(20);
		m_temp_queue = new LRU_Queue<HexTile*, mygreater>(20);
	}
	~CSession()
	{
		delete m_loginstate;
		delete m_lobbystate;
		delete m_roomstate;
		delete m_gamestate;
		delete m_userinfo;

		delete m_real_queue;
		delete m_temp_queue;
	}
	void Init();
	void End();

	t_UserInfo* GetUserInfo()
	{
		return m_userinfo;
	}
	void SetUserInfo(t_UserInfo _tuserinfo)
	{
		memcpy(&m_userinfo, &_tuserinfo, sizeof(t_UserInfo));
	}
	void SetUserInfo(TCHAR* _id, TCHAR* _pw, TCHAR* _nick, bool _flag)
	{
		ZeroMemory(m_userinfo->id, STRINGSIZE);
		ZeroMemory(m_userinfo->pw, STRINGSIZE);
		ZeroMemory(m_userinfo->nickname, STRINGSIZE);
		if (_id == NULL || _pw == NULL || _nick == NULL)
		{
			m_userinfo->is_login = _flag;
			return;
		}
		_tcsncpy(m_userinfo->id, _id, _tcslen(_id) * CODESIZE);
		_tcscpy(m_userinfo->pw, _pw);
		_tcscpy(m_userinfo->nickname, _nick);
		m_userinfo->is_login = _flag;
	}
	void SetSector(QuadNode* _nodesector)
	{
		m_sector = _nodesector;
		double curtime = 0;
		time_t last_update_time = time(NULL);
		unordered_set<CSector*>& viewlist = m_sector->GetViewSector();

		bool same = false;
		unordered_set<HexTile*> tilelist;
		list<HexTile*> suc_tile;
		list<HexTile*> new_tile;
		while (m_real_queue->Empty() == false)
		{
			same = false;
			HexTile* temp = m_real_queue->Front();
			m_real_queue->Pop();
			for (auto sector : viewlist)
			{
				tilelist = sector->GetTileList();
				auto itr = tilelist.find(temp);
				if (itr != tilelist.end())
				{   //리얼 큐랑 타일리스트 둘다 있는 경우
					//중복데이터면 시간갱신해서 넣기
					temp->SetRenderTime(last_update_time);
					HexTile* data = nullptr;
					if (m_temp_queue->Push(temp, data))
					{
						cout << data->GetTime() << " 이거 지워짐\n";
						delete data;
					}
					suc_tile.push_back(temp);
					same = true;
					break;
				}
			}
			if (!same)
			{   //리얼 큐에 있고 타일 리스트에 없는 경우=원래 시간으로 넣기.
				//다 돌았는데 tilelist에 없었다. real queue에만 있었다.
				HexTile* data = nullptr;
				if (m_temp_queue->Push(temp, data))
				{
					cout << data->GetTime() << " 이거 지워짐\n";
					delete data;
				}
			}
		}

		//리얼 큐에 없고 타일 리스트에만 있는경우
		for (auto sector : viewlist)
		{
			tilelist = sector->GetTileList();
			for (auto suc : suc_tile)
			{
				auto itr = tilelist.find(suc);
				if (itr != tilelist.end())
				{
					tilelist.erase(suc);
				}
			}
			for (auto tile : tilelist)
			{
				tile->SetRenderTime(last_update_time);
				HexTile* data = nullptr;
				if (m_temp_queue->Push(tile, data))
				{
					cout << data->GetID()<<"|"<<data->GetTime() << " 이거 지워짐\n";
					delete data;
				}
			}
		}
		
		LRU_Queue<HexTile*,mygreater>* tempqueue;
		tempqueue = m_real_queue;
		m_real_queue = m_temp_queue;
		m_temp_queue = tempqueue;
	}
	CSector* GetSector()
	{
		return m_sector;
	}
	CState* GetState() { return m_curstate; }
	CState* GetLoginState()
	{
		m_loginstate->Init();
		return m_loginstate;
	}
	CState* GetLobbyState()
	{
		m_lobbystate->Init();
		return m_lobbystate;
	}
	CState* GetRoomState()
	{
		m_roomstate->Init();
		return m_roomstate;
	}
	CState* GetGameState()
	{
		m_gamestate->Init();
		return m_gamestate;
	}
	void SetState(CState* _state)
	{
		m_curstate = _state;
	}
	void SetPlayer(int _index)
	{
		if (m_player == nullptr)
			m_player = new CPlayer(_index, m_userinfo->nickname, E_CharacterType::None, Vector3(0, 0, 0));
		else
			m_player->SetInfo(m_userinfo->nickname, E_CharacterType::None, Vector3(0, 0, 0));
	};
	int GetRoomID()
	{
		return m_roomid;
	}
	void SetRoomID(int _index)
	{
		m_roomid = _index;
	}
	int GetGameID()
	{
		return m_gameid;
	}
	void SetGameID(int _index)
	{
		m_gameid = _index;
	}
	CPlayer* GetPlayer() { return m_player; };
	const LRU_Queue<HexTile*, mygreater>& GetCurTiles();
private:
	t_UserInfo* m_userinfo;
	// STATE
	CState* m_curstate;
	CLoginState* m_loginstate;
	CLobbyState* m_lobbystate;
	CRoomState* m_roomstate;
	CGameState* m_gamestate;
	QuadNode* m_sector;
	LRU_Queue<HexTile*, mygreater>* m_real_queue;
	LRU_Queue<HexTile*,mygreater>* m_temp_queue;

	int m_roomid;
	int m_gameid;
	CPlayer* m_player;
	friend class CState;
	//int substate;
};

#endif
