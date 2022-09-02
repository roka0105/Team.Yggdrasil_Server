#include "pch.h"
#include "CPlayer.h"

const int CPlayer::GetID()
{
    return m_index;
}

TCHAR* CPlayer::Getname()
{
    return m_playername;
}

const E_CharacterType* CPlayer::GetType()
{
    return &m_character_type;
}

void CPlayer::SetType(E_CharacterType _type)
{
    m_character_type = _type;
}

const bool* CPlayer::GetReady()
{
    return& is_ready;
}

void CPlayer::SetReady(bool _ready)
{
    is_ready = _ready;
}

void CPlayer::SetInfo(TCHAR* _name, E_CharacterType _c_type, Vector3 _pos)
{
    m_obj_type = E_GameObject::Player;
    _tcscpy(m_playername, _name);
    m_character_type = _c_type;
    m_position = _pos;
    is_ready = false;
}

