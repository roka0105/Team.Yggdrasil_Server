#pragma once
#include "GameObject.h"

enum class E_CharacterType
{
	None,
	Defense,
	Attack,
	Support,
	Max
};
class CPlayer :public GameObject
{
public:
	CPlayer()
	{
		m_obj_type = E_GameObject::Player;
		m_index = -1;
		ZeroMemory(m_playername, STRINGSIZE);
		is_ready = false;
	}
	CPlayer(int _index,TCHAR* _name, E_CharacterType _c_type, Vector3 _pos)
	{
		m_obj_type = E_GameObject::Player;
		m_index = _index;
		_tcscpy(m_playername, _name);
		m_character_type = _c_type;
		m_position = _pos;
		is_ready = false;
	}
	const int GetID();
	TCHAR* Getname();
	const E_CharacterType* GetType();
	void SetType(E_CharacterType _type);
	const bool* GetReady();
	void SetReady(bool _ready);
	void SetInfo(TCHAR* _name, E_CharacterType _c_type, Vector3 _pos);
	Vector3 GetPos();
private:
	TCHAR m_playername[STRINGSIZE];
	E_CharacterType m_character_type;
	bool is_ready;
	Vector3 m_pos;

};

