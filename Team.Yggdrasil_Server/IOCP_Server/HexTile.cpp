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

void HexTile::operator=(UINT _time)
{
	m_time = _time;
}
