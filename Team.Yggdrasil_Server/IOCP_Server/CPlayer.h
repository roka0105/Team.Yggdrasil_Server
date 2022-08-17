#pragma once
#include "GameObject.h"

enum class E_CharacterType
{
    None,
    Attack,
    Defense,
    Support,
    Max
};
class CPlayer :public GameObject
{
public:
    CPlayer()
    {
        m_obj_type = E_GameObject::Player;
        m_index = createcount++;
        ZeroMemory(m_playername, STRINGSIZE);
    }
    CPlayer(TCHAR* _name,E_CharacterType _c_type,Vector3 _pos)
    {
        m_obj_type = E_GameObject::Player;
        m_index = createcount++;
        _tcscpy(m_playername, _name);
        m_character_type = _c_type;
        m_position = _pos;
    }
private:
    static int createcount;
    TCHAR m_playername[STRINGSIZE];
    E_CharacterType m_character_type;
};

