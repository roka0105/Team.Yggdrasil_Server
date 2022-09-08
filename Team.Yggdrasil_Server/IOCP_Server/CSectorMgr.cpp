#include "pch.h"
#include "CSectorMgr.h"
#include "CSector.h"
#include "GameObject.h"
#include "CProtocolMgr.h"
#include "CSession.h"
#include "CLock.h"
#include "CLockGuard.h"
#include "HexTile.h"

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
	m_lock = new CLock();
}
CSectorMgr::~CSectorMgr()
{
	delete m_lock;
}

void CSectorMgr::Init()
{
	
	
}

void CSectorMgr::End()
{
	
}
void CSectorMgr::AddQuadTree(t_GameInfo* _gameinfo, t_MapInfo* _mapinfo)
{
	CreateQuadTree(_gameinfo,_mapinfo);
}
#pragma region ver1

//void CSectorMgr::SendInit(CSession* _session)
//{
//    unsigned long protocol = 0;
//    CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::TEST));
//    Packing(protocol, *m_start_position, *m_end_position, *m_h_distance, *m_v_distance, m_sector_count, _session);
//}

#pragma endregion

#pragma region test ver2
void CSectorMgr::SendInit(UINT _gameid,CSession* _session,t_MapInfo* _mapinfo)
{
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::INIT));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CGameMgr::SUBPROTOCOL::SECTOR));

	list<Vector3> starts;
	Vector3 distance;
	QuadNode* root = m_roots[_gameid];
	
	for (int i = 0; i < QuadNode::GetCreateCount(); i++)
	{
		QuadNode* item = reinterpret_cast<QuadNode*>(*SerchNode(root, i, 0,_mapinfo));
		starts.push_back(item->GetStartPos());
		if (i == 0)
		{
			distance = item->GetDistance();
		}
	}

	Packing(protocol, starts, distance, _session);
}
QuadNode* CSectorMgr::SerchObjectNode(t_GameInfo* _gameinfo, Vector3 _pos)
{
	QuadNode* node = reinterpret_cast<QuadNode*>(*SerchObjectNode(m_roots[_gameinfo->m_id], _pos, 0, _gameinfo->m_mapinfo));
	return node;
}
#pragma endregion
void CSectorMgr::CreateQuadTree(t_GameInfo* _gameinfo,t_MapInfo* _mapinfo)
{
	
	Vector3 distance((_mapinfo->m_h_mapsize), 0, (_mapinfo->m_v_mapsize));
	Vector3 senter_pos(_mapinfo->m_start_position.x + (distance.x / 2), _mapinfo->m_default_y, _mapinfo->m_start_position.z - (distance.z / 2));
	distance.x /= 2;
	distance.z /= 2;
	QuadNode* root = new QuadNode(Vector3(), distance);

	CreateChildren(root, senter_pos, distance, 0,_gameinfo);

	{
		CLockGuard<CLock> lock(m_lock);
		m_roots.insert({ _gameinfo->m_id,root });
	}
	
	//view sector list 도 setting 하는거 만들기
	SetViewNode(root, 0,_mapinfo, _gameinfo->m_id);

}

void CSectorMgr::CreateChildren(QuadNode* _parent, Vector3 _senterpos, Vector3 _distance, int _curdepth, t_GameInfo* _gameinfo)
{
	t_MapInfo* _mapinfo = _gameinfo->m_mapinfo;
	if (_mapinfo->m_depth == _curdepth)
	{
		//leaf node id 
		_parent->SetID();
		/*Vector3 vec = _parent->GetDistance();
		vec.x += 1;
		vec.z += 1;
		_parent->SetDistance(vec);*/
		_gameinfo->m_leaf_nodes.insert({ _parent->GetID(),_parent });
		return;
	}
	Vector3 distance(_distance.x /_mapinfo->m_squared_value, 0, _distance.z / _mapinfo->m_squared_value);
 	Vector3* senterpos = nullptr;
	QuadNode* child = nullptr;
	for (int i = 0; i < _mapinfo->m_squared_value * 2; i++)
	{
		switch (i)
		{
		case 0:// left up
			senterpos = new Vector3(_senterpos.x - distance.x, _mapinfo->m_default_y, _senterpos.z + distance.z);
			child = new QuadNode(*senterpos, distance);
			break;
		case 1://right up
			senterpos = new Vector3(_senterpos.x + distance.x, _mapinfo->m_default_y, _senterpos.z + distance.z);
			child = new QuadNode(*senterpos, distance);
			break;
		case 2://left down
			senterpos = new Vector3(_senterpos.x - distance.x, _mapinfo->m_default_y, _senterpos.z - distance.z);
			child = new QuadNode(*senterpos, distance);
			break;
		case 3://right down
			senterpos = new Vector3(_senterpos.x + distance.x, _mapinfo->m_default_y, _senterpos.z - distance.z);
			child = new QuadNode(*senterpos, distance);
			break;
		}

		if (child == nullptr)
		{
			//메모리할당 실패
			return;
		}
		_parent->AddChildren(child);
		CreateChildren(child, *senterpos, distance, _curdepth + 1,_gameinfo);
		if (senterpos != nullptr)
			delete senterpos;
	}
}
#pragma region viewsector stting ver 1
void CSectorMgr::SetViewNode(QuadNode* _parent, int _curdepth, t_MapInfo* _mapinfo,UINT _gameid)
{
    if (_mapinfo->m_depth == _curdepth)
    {
        Vector3 start = _parent->GetStartPos();
        Vector3 distance = _parent->GetDistance();
		if (_parent->GetID() == 8)
		{
			printf("id=8");
		}
        Vector3 position;
        QuadNode** viewnode = nullptr;

        //시야의 최대치는 8이다.
        for (int i = 0; i < _mapinfo->m_eyesight; i++)
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
            viewnode = SerchNode(m_roots[_gameid], position, 0,_mapinfo);
            if (viewnode == nullptr)
                continue;
            else 
                _parent->SetViewSector(*viewnode);
        }
		_parent->SetViewSector(_parent);
        return;
    }
    for (int i = 0; i < _mapinfo->m_depth * _mapinfo->m_squared_value; i++)
    {
        QuadNode* child = _parent->GetChildNode(i);
        SetViewNode(child, _curdepth+1,_mapinfo,_gameid);
    }

}
#pragma endregion
#pragma region viewsector stting ver2
void CSectorMgr::SetViewNode(CSession* _session)
{
	/*
	CPlayer* player = _session->GetPlayer();
	Vector3 playerpos = player->GetPos();
	float firstcheck_x = 45;
	float firstcheck_z = 10;
	float secondcheck_x = 116;
	float secondcheck_z = 144;
	Vector3 start_check_1(playerpos.x-firstcheck_x,0,playerpos.z+firstcheck_x);
	Vector3 end_check_1(playerpos.x + firstcheck_x, 0, playerpos.z - firstcheck_x);
	Vector3 start_check_2(playerpos.x - secondcheck_x, 0, playerpos.z + firstcheck_z + secondcheck_z);
	Vector3 end_check_2(playerpos.x + secondcheck_x, 0, playerpos.z + firstcheck_z);
	//범위 비교 
	//1차 검사 : 플레이어위치 기준 x= +-47 z= +-11 에 들어오는 섹터들 view 에 넣기
	//2차 검사 : 플레이어위치 + z(11) 기준 x= +-118 z= +146에 들어오는 섹터들 view 에 넣기
	for (int i = 0; i < m_sector_count; i++)
	{
		QuadNode* node = reinterpret_cast<QuadNode*>(*SerchNode(root, i, 0));
		if (node->IsInSector(start_check_1) || node->IsInSector(end_check_1)||node->IsInSector(start_check_2)||node->IsInSector(end_check_2))
		{
			_session->GetSector()->SetViewSector(node);
		}
	}
	*/
	/*if (_curdepth == *m_depth)
	{
	}
	QuadNode* child = nullptr;
	int size = _parent->Child_Size();
	for (int i = 0; i < size; i++)
	{
		child = _parent->GetChildNode(i);
		if (child->GetID() == -1)
		{
			SetViewNode(child, _curdepth + 1);
		}
	}*/
}
#pragma endregion

void CSectorMgr::AddObjectNode(QuadNode* _parent, GameObject* obj, int curdepth)
{/*
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
			AddObjectNode(child, obj, curdepth + 1);
	}
	*/
}

void CSectorMgr::RemoveObjectNode(QuadNode* _parent, GameObject* _obj, int _curdepth)
{
	/*
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
			RemoveObjectNode(child, _obj, _curdepth + 1);
	}
	*/
}

QuadNode** CSectorMgr::SerchNode(QuadNode* _parent, Vector3 _pos, int _curdepth, t_MapInfo* _mapinfo)
{
	if (_mapinfo->m_depth == _curdepth)
	{
		if (_mapinfo->m_start_position.x <= _pos.x && _mapinfo->m_end_position.x >= _pos.x
			&& _mapinfo->m_start_position.z >= _pos.z && _mapinfo->m_end_position.z <= _pos.z)

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
			item = SerchNode(child, _pos, _curdepth + 1,_mapinfo);
		if (item != nullptr)
			return item;
	}
	return nullptr;
}

QuadNode** CSectorMgr::SerchObjectNode(QuadNode* _parent, Vector3 _pos, int _curdepth, t_MapInfo* _mapinfo)
{
	if (_mapinfo->m_depth == _curdepth)
	{
		if (_mapinfo->m_start_position.x <= _pos.x && _mapinfo->m_end_position.x >= _pos.x
			&& _mapinfo->m_start_position.z >= _pos.z && _mapinfo->m_end_position.z <= _pos.z)

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
		if (child->IsInSector_Obj(_pos))
			item = SerchObjectNode(child, _pos, _curdepth + 1, _mapinfo);
		if (item != nullptr)
			return item;
	}
	return nullptr;
}

QuadNode** CSectorMgr::SerchNode(QuadNode* _parent, int _id, int _curdepth, t_MapInfo* _mapinfo)
{
	if (_curdepth == _mapinfo->m_depth)
	{
		if (_parent->GetID() == _id)
			return &_parent;
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
		item = SerchNode(child, _id, _curdepth + 1, _mapinfo);
		if (item != nullptr)
			return item;
	}
	return nullptr;
}

bool CSectorMgr::IsInView(QuadNode* _parent, Vector3 _startview, Vector3 _endview)
{
	/*
	for (int i = 0; i < m_sector_count; i++)
	{
		QuadNode* node = reinterpret_cast<QuadNode*>(*SerchNode(root, i, 0));
		if (node->IsInSector(_startview) || node->IsInSector(_endview))
		{
			return true;
		}
	}
    */
	return false;
}

void CSectorMgr::PlayerSendPacket(CSession* _session, unsigned long _protocol, bool moveflag)
{
	/*
	플레이어 정보를 전송합니다.
	moveflag=true이면 이동이 있었다는걸로 섹터 이동이 있는지 체크 후
	섹터 이동이 있을 시 섹터 정보들을 변경시킵니다.
	*/
}

void CSectorMgr::TestSendViewSectorProcess(CSession* _session, t_GameInfo* _gameinfo,Vector3 _objpos)
{
	/*지금 player가 있는 sector의 viewlist만 전송하는데 렌더할 때는 자기 자신의 정보까지 보내야 함.*/
	/*byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	Vector3 obj_pos;
	UnPacking(data, obj_pos);*/
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::TEST));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CGameMgr::SUBPROTOCOL::SECTOR));
	int test_sector_index = 9;

	list<Vector3> starts;
	Vector3 distance;

	QuadNode* sector = reinterpret_cast<QuadNode*>(*SerchObjectNode(m_roots[_gameinfo->m_id], _objpos, 0, _gameinfo->m_mapinfo));
	
	
	unordered_set<CSector*> viewlist = sector->GetViewSector();
	int count = 0;
	for (auto viewnode : viewlist)
	{
		if (count == 0)
		{
			distance = viewnode->GetDistance();
		}
		count++;
		starts.push_back(viewnode->GetStartPos());
	}
	Packing(protocol, starts, distance, _session);
}

void CSectorMgr::TestSendViewTileProcess(CSession* _session, t_GameInfo* _gameinfo,Vector3 _objpos)
{
	//byte data[BUFSIZE];
	//ZeroMemory(data, BUFSIZE);
	//_session->UnPacking(data);
	//Vector3 obj_pos;
	//UnPacking(data, obj_pos);
	unsigned long protocol = 0;
	CProtocolMgr::GetInst()->AddMainProtocol(&protocol, static_cast<unsigned long>(MAINPROTOCOL::TEST));
	CProtocolMgr::GetInst()->AddSubProtocol(&protocol, static_cast<unsigned long>(CGameMgr::SUBPROTOCOL::Object));
	CProtocolMgr::GetInst()->AddDetailProtocol(&protocol, static_cast<unsigned long>(CGameMgr::DETAILPROTOCOL::Tile));
	list<Vector3> starts;
	Vector3 distance;

	//QuadNode* sector = reinterpret_cast<QuadNode*>(*SerchObjectNode(m_roots[_gameinfo->m_id], _objpos, 0, _gameinfo->m_mapinfo));


	//unordered_set<CSector*> viewlist = sector->GetViewSector();
	////viewlist.push_back(sector);

	//int count = 0;
	//for (auto viewnode : viewlist)
	//{
	//	for (auto tile : viewnode->GetTileList())
	//	{
	//		Vector3 pos = tile->GetSenterPos();
	//		starts.push_back(pos);
	//	}
	//}
	LRU_Queue<HexTile*, mygreater> render_queue = _session->GetCurTiles();
	int size = render_queue.Size();
	for(int i=0;i<size;i++)
	{
		HexTile* tile = render_queue[i];
		Vector3 pos = tile->GetSenterPos();
		starts.push_back(pos);
	}
	Packing(protocol, starts, distance, _session);
}

void CSectorMgr::TestPlayerMove(CSession* _session, t_GameInfo* _gameinfo)
{
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	_session->UnPacking(data);
	Vector3 obj_pos;
	UnPacking(data, obj_pos);
	CPlayer* player = _session->GetPlayer();
	player->SetVector(obj_pos);
	QuadNode* sector = SerchObjectNode(_gameinfo,obj_pos);
	_session->SetSector(sector);
	TestSendViewSectorProcess(_session,_gameinfo,obj_pos);
	TestSendViewTileProcess(_session, _gameinfo, obj_pos);
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

void CSectorMgr::Packing(unsigned long _protocol, list<Vector3>& _starts, Vector3& _distances, CSession* _session)
{
	int size = 0;
	byte data[BUFSIZE];
	ZeroMemory(data, BUFSIZE);
	byte* ptr = data;

	int count = _starts.size();
	memcpy(ptr, &count, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, &_distances.x, sizeof(float));
	ptr += sizeof(float);
	size += sizeof(float);
	memcpy(ptr, &_distances.y, sizeof(float));
	ptr += sizeof(float);
	size += sizeof(float);
	memcpy(ptr, &_distances.z, sizeof(float));
	ptr += sizeof(float);
	size += sizeof(float);
	for (int i = 0; i < count; i++)
	{
		Vector3 temp = _starts.front();
		memcpy(ptr, &temp.x, sizeof(float));
		ptr += sizeof(float);
		size += sizeof(float);
		memcpy(ptr, &temp.y, sizeof(float));
		ptr += sizeof(float);
		size += sizeof(float);
		memcpy(ptr, &temp.z, sizeof(float));
		ptr += sizeof(float);
		size += sizeof(float);
		_starts.pop_front();
	}
	ptr = data;
	_session->Packing(_protocol, ptr, size);
}

void CSectorMgr::UnPacking(byte* _recvbuf, Vector3& _pos)
{
	byte* ptr = _recvbuf;
	memcpy(&_pos.x, ptr, sizeof(float));
	ptr += sizeof(float);
	memcpy(&_pos.y, ptr, sizeof(float));
	ptr += sizeof(float);
	memcpy(&_pos.z, ptr, sizeof(float));
	ptr += sizeof(float);
}


