#include "pch.h"
#include "CSector.h"
#include "HexTile.h"
int QuadNode::create_count = 0;

int QuadNode::GetCreateCount()
{
	return create_count;
}

QuadNode::QuadNode()
{

}

QuadNode::QuadNode(Vector3 _senter_pos, Vector3 _distance) :CSector(_senter_pos, _distance)
{
	m_id = -1;
}

QuadNode::~QuadNode()
{
	while (m_children.size() != 0)
	{
		QuadNode* item = m_children.back();
		m_children.pop_back();
		delete item;
	}
}

void QuadNode::AddChildren(QuadNode* _child_node)
{
	_child_node->m_parent = this;
	m_children.push_back(_child_node);
}

QuadNode* QuadNode::GetChildNode(int index)
{
	int i = 0;
	for (vector<QuadNode*>::iterator itr = m_children.begin(); itr != m_children.end(); itr++)
	{
		if (i == index)
		{
			return *itr;
		}
		i++;
	}
	return nullptr;
}

void QuadNode::SetID()
{
	m_id = create_count++;
}

int QuadNode::GetID()
{
	return m_id;
}

QuadNode* QuadNode::GetParent()
{
	return m_parent;
}

CSector::CSector()
{
}

CSector::CSector(Vector3 _senter_pos, Vector3 _distance)
{
	m_senter_pos = _senter_pos;
	m_distance = _distance;
	m_start_pos.x = _senter_pos.x - _distance.x;
	m_start_pos.y = _senter_pos.y;
	m_start_pos.z = _senter_pos.z + _distance.z;
}

CSector::~CSector()
{
}

void CSector::AddObject(GameObject* _object)
{
	if (dynamic_cast<HexTile*>(_object) != nullptr)
	{
		HexTile* hex = dynamic_cast<HexTile*>(_object);
		m_tile_list.insert(hex);
	}
	else
	{
		//해당하는 위치의 tile 검색
		//해당 tile에 addobject
		for (HexTile* tile : m_tile_list)
		{
			if (tile->InHex(_object->GetVector()))
			{
				tile->AddObject(_object);
				break;
			}
		}
	}
	
}

const Vector3 CSector::GetDistance()
{
	return m_distance;
}

void CSector::SetDistance(Vector3 _pos)
{
	m_distance = _pos;
}

const Vector3 CSector::GetSenter()
{
	return m_senter_pos;
}

const Vector3 CSector::GetStartPos()
{
	return m_start_pos;
}
BOOL CSector::IsInSector(const Vector3 _obj_pos)
{
	if (_obj_pos.x >= m_start_pos.x && _obj_pos.x <= m_start_pos.x + m_distance.x
 		&& _obj_pos.z <= m_start_pos.z && _obj_pos.z >= m_start_pos.z - m_distance.z)
	{
		return true;
	}
	return false;
}

BOOL CSector::IsInSector_Obj(const Vector3& _obj_pos)
{
	if (_obj_pos.x >= m_start_pos.x && _obj_pos.x <= m_start_pos.x + m_distance.x*2
		&& _obj_pos.z <= m_start_pos.z && _obj_pos.z >= m_start_pos.z - m_distance.z*2)
	{
		return true;
	}
	return false;
}

BOOL CSector::IsInSector_Direction(const Vector3 _obj_pos, E_NodeType _type)
{
	/*
	switch (_type)
	{
	case E_NodeType::Left:
		if (_obj_pos.x >= m_start_pos.x - m_distance.x * 2 && _obj_pos.x < m_start_pos.x
			&& _obj_pos.z <= m_start_pos.z && _obj_pos.z > m_start_pos.z - m_distance.z * 2)
		{
			return true;
		}
		break;
	case E_NodeType::LeftUp:
		if (_obj_pos.x >= m_start_pos.x - m_distance.x * 2 && _obj_pos.x < m_start_pos.x
			&& _obj_pos.z <= m_start_pos.z + m_distance.z * 2 && _obj_pos.z > m_start_pos.z)
		{
			return true;
		}
		break;
	case E_NodeType::LeftDown:
		if (_obj_pos.x >= m_start_pos.x - m_distance.x * 2 && _obj_pos.x < m_start_pos.x
			&& _obj_pos.z <= m_start_pos.z - m_distance.z * 2 && _obj_pos.z > m_start_pos.z - m_distance.z * 4)
		{
			return true;
		}
		break;
	case E_NodeType::Right:
		if (_obj_pos.x >= m_start_pos.x && _obj_pos.x < m_start_pos.x + m_distance.x * 2
			&& _obj_pos.z <= m_start_pos.z && _obj_pos.z > m_start_pos.z - m_distance.z * 2)
		{
			return true;
		}
		break;
	case E_NodeType::RightUp:
		if (_obj_pos.x >= m_start_pos.x + m_distance.x * 2 && _obj_pos.x < m_start_pos.x + m_distance.x * 4
			&& _obj_pos.z <= m_start_pos.z + m_distance.z * 2 && _obj_pos.z > m_start_pos.z)
		{
			return true;
		}
		break;
	case E_NodeType::RightDown:
		if (_obj_pos.x >= m_start_pos.x + m_distance.x * 2 && _obj_pos.x < m_start_pos.x + m_distance.x * 4
			&& _obj_pos.z <= m_start_pos.z - m_distance.z * 2 && _obj_pos.z > m_start_pos.z - m_distance.z * 4)
		{
			return true;
		}
		break;
	}*/

	return false;
}

void CSector::SetViewSector(CSector* _node)
{
	m_view_sectorlist.insert(_node);
}

unordered_set<CSector*>& CSector::GetViewSector()
{
	return m_view_sectorlist;
}

unordered_set<HexTile*>& CSector::GetTileList()
{
	return m_tile_list;
}

int QuadNode::Child_Size()
{
	return m_children.size();
}
