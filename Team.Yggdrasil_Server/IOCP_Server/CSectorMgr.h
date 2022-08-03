#pragma once
class CSession;
class CSector;
class CMonster;
class CPlayer;
class CSectorMgr
{
public:
    //플레이어 샌드패킷
    void PlayerSendPacket(CSession* _session,CPlayer* _player, unsigned long _protocol, bool _moveflag);
   
    //몬스터 샌드패킷
    void MonsterSendPacket(CSession* _session, CMonster* _monster, unsigned long _protocol, bool _moveflag);
    //아이템 샌드패킷
    void ItemSendPacket(CSession* _session, unsigned long _protocol, bool _onoff_flag);
private:
    list<CSector> m_sectorlist;
};

