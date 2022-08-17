#include "pch.h"
#include "CSectorMgr.h"
#include "CSector.h"
#include "GameObject.h"
#include "CProtocolMgr.h"
#include "CSession.h"
CSectorMgr* CSectorMgr::m_instance = nullptr;

CSectorMgr* CSectorMgr::GetInst()
{
    return m_instance;
}

void CSectorMgr::Create()
{
    if (m_instance == nullptr)
        m_instance = new CSectorMgr();
}

void CSectorMgr::Destory()
{
    delete m_instance;
}
CSectorMgr::CSectorMgr()
{

}
CSectorMgr::~CSectorMgr()
{
}

void CSectorMgr::Init()
{
    //db에서 값 읽어와서 읽어온 값으로 설정하도록 하기
    //현재는 임시로 값 넣기
    m_start_position = new Vector3(-45, m_default_y, 38);
    m_end_position = new Vector3(195, m_default_y, -142);
    m_tile_distance = new float(30);
    m_h_mapsize = new int(240);
    m_v_mapsize = new int(180);
    m_sector_count = 1;
    m_depth = new int(2);
    m_squared_value = new int(2);
    m_eyesight = new int(8);

    for (int i = 0; i < *m_depth; i++)
    {
        m_sector_count *= (*m_squared_value);
    }
    m_h_distance = new float(*m_h_mapsize / m_sector_count);
    m_v_distance = new float(*m_v_mapsize / m_sector_count);
    CreateQuadTree();
}

void CSectorMgr::End()
{
    delete m_tile_distance;
    delete m_h_mapsize;
    delete m_v_mapsize;
    delete m_squared_value;
    delete m_depth;
    delete m_h_distance;
    delete m_v_distance;
}

void CSectorMgr::SendInit(CSession* _session)
{
    unsigned long protocol = 0;
    CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::TEST));
    Packing(protocol, *m_start_position, *m_end_position, *m_h_distance, *m_v_distance, m_sector_count, _session);
}

void CSectorMgr::CreateQuadTree()
{
    Vector3 distance((*m_h_mapsize), 0, (*m_v_mapsize));
    Vector3 senter_pos(m_start_position->x+(distance.x/2) , m_default_y, m_start_position->z-(distance.z/2) );
    distance.x /= 2;
    distance.z /= 2;
    root = new QuadNode(Vector3(), distance);

    SetChildren(root, senter_pos, distance, 0);
    //view sector list 도 setting 하는거 만들기
    SetViewNode(root,0);
}

void CSectorMgr::SetChildren(QuadNode* _parent, Vector3 _senterpos, Vector3 _distance, int _curdepth)
{
    if (*m_depth == _curdepth)
        return;
    Vector3 distance(_distance.x / (*m_squared_value), 0, _distance.z / (*m_squared_value));
    Vector3* senterpos = nullptr;
    QuadNode* child = nullptr;
    for (int i = 0; i < (*m_squared_value) * 2; i++)
    {
        switch (i)
        {
        case 0:// left up
            senterpos = new Vector3(_senterpos.x - distance.x, m_default_y, _senterpos.z + distance.z);
            child = new QuadNode(*senterpos, distance);
            break;
        case 1://right up
            senterpos = new Vector3(_senterpos.x + distance.x, m_default_y, _senterpos.z + distance.z);
            child = new QuadNode(*senterpos, distance);
            break;
        case 2://left down
            senterpos = new Vector3(_senterpos.x - distance.x, m_default_y, _senterpos.z - distance.z);
            child = new QuadNode(*senterpos, distance);
            break;
        case 3://right down
            senterpos = new Vector3(_senterpos.x + distance.x, m_default_y, _senterpos.z - distance.z);
            child = new QuadNode(*senterpos, distance);
            break;
        }
        
        if (child == nullptr)
        {
            //메모리할당 실패
            return;
        }
        _parent->AddChildren(child);
        SetChildren(child, *senterpos, distance, _curdepth+1);
        if (senterpos != nullptr)
            delete senterpos;
    }
}

void CSectorMgr::SetViewNode(QuadNode* _parent, int _curdepth)
{
    if (*m_depth == _curdepth)
    {
        Vector3 start = _parent->GetStartPos();
        Vector3 distance = _parent->GetDistance();

        Vector3 position;
        QuadNode** viewnode = nullptr;

        //시야의 최대치는 8이다.
        for (int i = 0; i < *m_eyesight; i++)
        {
            switch (static_cast<E_NodeType>(i))
            {
            case E_NodeType::Left:
                //left node
                position.x = start.x - distance.x*2;
                position.y = start.y;
                position.z = start.z;
                break;
            case E_NodeType::LeftUp:
                //left up node
                position.z = start.z + distance.z*2;
                break;
            case E_NodeType::LeftDown:
                //left down node
                position.z = start.z - distance.z*2;
                break;
            case E_NodeType::Right:
                //right node
                position.x = start.x + distance.x*2;
                position.y = start.y;
                position.z = start.z;
                break;
            case E_NodeType::RightUp:
                //right up node
                position.z = start.z + distance.z*2;
                break;
            case E_NodeType::RightDown:
                //right down node
                position.z = start.z - distance.z*2;
                break;
            case E_NodeType::Up:
                //senter up
                position.x = start.x;
                position.y = start.y;
                position.z = start.z + distance.z*2;
                break;
            case E_NodeType::Down:
                //senter down
                position.z = start.z - distance.z*2;
                break;
            }
            viewnode = SerchNode(root, position, 0,static_cast<E_NodeType>(i));
            if (viewnode == nullptr)
                continue;
            else 
                _parent->SetViewSector(*viewnode);
        }
        return;
    }
    for (int i = 0; i < (*m_depth) * (*m_squared_value); i++)
    {
        QuadNode* child = _parent->GetChildNode(i);
        SetViewNode(child, _curdepth+1);
    }

}

void CSectorMgr::AddObjectNode(QuadNode* _parent, GameObject* obj, int curdepth)
{
    Vector3 obj_pos = obj->GetVector();
    if (*m_depth == curdepth)
    {
        //마지막 깊이에서 검색된 노드의 위치안에 오브젝트 위치가 속하면 해당 노드에 오브젝트 추가.
        if (_parent->IsInSector(obj_pos))
        {
            _parent->AddObject(obj);
        }
    }
    QuadNode* child = nullptr;
    int size = _parent->Child_Size();
    for (int i = 0; i < size; i++)
    {
        child = _parent->GetChildNode(i);
        //아직 최대 깊이까지 도달 안했을때 
        //부모의 범위 내에 obj_pos가 일치할 때만 해당 영역의 
        //자식 노드를 순회하도록 한다.
        if (child->IsInSector(obj_pos))
            AddObjectNode(child, obj, curdepth+1);
    }

}

void CSectorMgr::RemoveObjectNode(QuadNode* _parent, GameObject* _obj, int _curdepth)
{
    Vector3 obj_pos = _obj->GetVector();
    if (*m_depth == _curdepth)
    {
        //마지막 깊이에서 검색된 노드의 위치안에 오브젝트 위치가 속하면 해당 노드에 오브젝트 추가.
        if (_parent->IsInSector(obj_pos))
        {
            _parent->AddObject(_obj);
        }
    }
    QuadNode* child = nullptr;
    int size = _parent->Child_Size();
    for (int i = 0; i < size; i++)
    {
        child = _parent->GetChildNode(i);
        //아직 최대 깊이까지 도달 안했을때 
        //부모의 범위 내에 obj_pos가 일치할 때만 해당 영역의 
        //자식 노드를 순회하도록 한다.
        if (child->IsInSector(obj_pos))
            RemoveObjectNode(child, _obj, _curdepth+1);
    }
}

QuadNode** CSectorMgr::SerchNode(QuadNode* _parent, Vector3 _pos, int _curdepth,E_NodeType _type)
{
    if (*m_depth == _curdepth)
    {
        if (m_start_position->x <= _pos.x && m_end_position->x >= _pos.x
            && m_start_position->z >= _pos.z && m_end_position->z <= _pos.z)

        {
            //if (_parent->IsInSector_Direction(_pos, _type))
            //{

                return &_parent;
            //}
        }
        return nullptr;
    }
    QuadNode* child = nullptr;
    QuadNode** item = nullptr;
    int size = _parent->Child_Size();
    for (int i = 0; i < size; i++)
    {
        child = _parent->GetChildNode(i);
        if (child == nullptr)
        {
            return nullptr;
        }
        if (child->IsInSector(_pos))
        item = SerchNode(child, _pos, _curdepth+1,_type);
        if (item != nullptr)
            return item;
    }
    return nullptr;
}

void CSectorMgr::PlayerSendPacket(CSession* _session, unsigned long _protocol, bool moveflag)
{
    /*
    플레이어 정보를 전송합니다.
    moveflag=true이면 이동이 있었다는걸로 섹터 이동이 있는지 체크 후
    섹터 이동이 있을 시 섹터 정보들을 변경시킵니다.
    */
}

void CSectorMgr::Packing(unsigned long _protocol, Vector3 _startpos, Vector3 _endpos, float _h_distance, float _v_distance, int _sectorcount, CSession* _session)
{
    int size = 0;
    byte data[BUFSIZE];
    ZeroMemory(data, BUFSIZE);
    byte* ptr = data;

    memcpy(ptr, &_startpos.x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_startpos.y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_startpos.z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_endpos.x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_endpos.y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_endpos.z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_h_distance, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_v_distance, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    memcpy(ptr, &_sectorcount, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);
    _session->Packing(_protocol, data, size);
}


