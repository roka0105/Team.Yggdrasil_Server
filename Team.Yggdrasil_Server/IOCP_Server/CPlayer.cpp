#include "pch.h"
#include "CPlayer.h"
int CPlayer::createcount = 0;

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
