#pragma once
#include "pch.h"
#include "CSector.h"

class CSector;
class GameObject;
class CSession;
class CSectorMgr
{
public:
    static CSectorMgr* GetInst();
    static void Create();
    static void Destory();
private:
    static CSectorMgr* m_instance;
    CSectorMgr();
    ~CSectorMgr();
public:
    void Init();
    void End();
    void SendInit(CSession* _session);
    void CreateQuadTree();
    void SetChildren(QuadNode* _parent,Vector3 _senterpos,Vector3 _distance,int _curdepth);
    void SetViewNode(QuadNode* _parent, int _curdepth);
    void SetViewNode(CSession* _session);
    void AddObjectNode(QuadNode* _parent,GameObject* obj,int _curdepth);
    void RemoveObjectNode(QuadNode* _parent,GameObject* obj, int _curdepth);
    QuadNode** SerchNode(QuadNode* _parent,Vector3 _pos,int _curdepth,E_NodeType _postype);
    QuadNode** SerchNode(QuadNode* _parent, int _id, int _curdepth);
    bool IsInView(QuadNode* _parent,Vector3 _startview,Vector3 _endview);
    //플레이어 샌드패킷
    void PlayerSendPacket(CSession* _session, unsigned long _protocol, bool moveflag);
    //void checksector()
    //몬스터 샌드패킷
    //void MonsterSendPacket(CSession*,CMonster*, Protocol,moveflag)
    //아이템 샌드패킷
    //void ItemSendPacket(CSession*,Protocol,_onoff_flag)
public:
    void Packing(unsigned long _protocol,Vector3 _startpos,Vector3 _endpos, float _h_distance,float _v_distance,int _sectorcount, CSession* _session);
    void Packing(unsigned long _protocol,list<Vector3>& _starts,Vector3& _distances,CSession* _session);
private:
    list<CSector*> m_sectorlist;
    Vector3* m_start_position;
    Vector3* m_end_position;
    /*
      모드마다 맵 사이즈, vector값, sector 등분, tree depth 가 다를수 있기 때문에
      const* 로 선언해놓고 Init() 할때 db에서 모드에 대한 정보를 읽어온 뒤
      그거에 맞게 섹터 나누고 설정 하도록 했다.
      즉 const* 이기 때문에 init할때 설정해놓은 값에서 변경되지 않을것.
      변경되는 경우는 게임 끝나고 End 처리 후 새로운 게임에서 Init할때.
     */
    const float* m_tile_distance; //= 30;// 육각형 타일 가로 세로 길이
    const int* m_h_mapsize; //= 240;
    const int* m_v_mapsize; //= 180;
    int m_sector_count; //현재 몇개의 섹터로 나뉜지 총 섹터 갯수
    const int* m_squared_value; // 몇등분 할 것인지. m_depth만큼 곱할것임. ex  squared=2 depth=2 => 4 가로세로 4등분. 
    const int* m_depth; //= 1;           // tree의 깊이
    const int* m_eyesight;

    const float* m_h_distance;
    const float* m_v_distance;

    const float m_default_y = 3;

    QuadNode* root;
};

