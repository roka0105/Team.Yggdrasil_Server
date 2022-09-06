#pragma once
#include "MemoryPool_2.h"
class GameObject;
class HexTile;
class CSector :public MemoryPool_2
{
protected:
    CSector();
    CSector(Vector3 _senter_pos,Vector3 _distance);
    
    virtual ~CSector();
public:
    void AddObject(GameObject* _object);      // 오브젝트 등록
    void SetArea(Vector3 _senter_pos);
    const Vector3 GetDistance();
    void SetDistance(Vector3 _pos);
    const Vector3 GetSenter();
    const Vector3 GetStartPos();
    BOOL IsInSector_Direction(const Vector3 _obj_pos,E_NodeType _type); 
    BOOL IsInSector(const Vector3 _obj_pos);// 오브젝트가 노드안에 있는지 체크
    BOOL IsInSector_Obj(const Vector3& _obj_pos);
    void SetViewSector(CSector* _node);
    unordered_set<CSector*>& GetViewSector();
    unordered_set<HexTile*>& GetTileList();
private:
    unordered_set<CSector*> m_view_sectorlist;
    unordered_set<HexTile*> m_tile_list;
    Vector3 m_start_pos;
    Vector3 m_senter_pos;         // 노드의 중심 위치
    Vector3 m_distance;           // 밑변/2 
};
class QuadNode : public CSector
{
public:
    static int GetCreateCount();
public:
    QuadNode();
    QuadNode(Vector3 _senter_pos, Vector3 _distance);
    virtual ~QuadNode()final;
    void AddChildren(QuadNode* _child_node);// 자식노드 등록  
    QuadNode* GetChildNode(int index);              // 자식노드 가져오기

    void SetID();
    int GetID();
    void SetParent(QuadNode* _parent_node);// 부모노드 설정
    QuadNode* GetParent();                 // 부모노드 가져오기

    void CullNode(BOOL _culling);          // 노드 컬링 여부
    BOOL IsCulled();                       // 노드가 컬링상태인지 판별.
    void Render();                         // 노드 정보 렌더링

    int Child_Size();
private:
    static int create_count;
    int m_id;
    vector<QuadNode*> m_children; // 자식노드
    QuadNode* m_parent;           // 부모노드
    BOOL m_is_culled;             // 컬링 여부
    const float m_limit_depth = 4;// 트리의 최대 깊이  
    E_NodeType m_type;
};