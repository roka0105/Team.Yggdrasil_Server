#include "pch.h"
#include "HexTile.h"
int HexTile::count = 0;

bool HexTile::InHex(Vector3 _objpos)
{
	if (abs(_objpos.x - m_senter.x) < m_radius && abs(_objpos.z - m_senter.z) < m_radius)
	{
		return true;
	}
	else
		return false;
}

Vector3 HexTile::GetSenterPos()
{
	return m_senter;
}

const UINT& HexTile::GetTime()
{
	return m_time;
}

void HexTile::AddObject(GameObject* _obj)
{
	m_objects.push_back(_obj);
}

void HexTile::SetRenderTime(UINT _time)
{
	m_time = _time;
}

bool HexTile::operator>(const HexTile& _tile)
{
	return this->m_time>_tile.m_time;
}

bool HexTile::operator<(const HexTile& _tile)
{
	return this->m_time < _tile.m_time;
}

bool HexTile::operator==(const HexTile& _tile)
{
	return this->m_time == _tile.m_time;
}

//void HexTile::operator=(UINT _time)
//{
//	m_time = _time;
//}
