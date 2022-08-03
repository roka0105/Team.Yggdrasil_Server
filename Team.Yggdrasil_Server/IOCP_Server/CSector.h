#pragma once
class CPlayer;
class CMonster;
class CBoss;
class CSpirit;
class CObject;
class CItem;
class CSector
{
public:
    CSector();
private:
    list<CPlayer> m_playerlist;
    list<CMonster> m_monsterlist;
    CBoss* m_boss;
    list<CSpirit>   m_spiritlist;
    list<CObject>   m_objectlist;
    list<CItem>     m_itemlist;
    list<CSector>   m_view_sectorlist;
};

