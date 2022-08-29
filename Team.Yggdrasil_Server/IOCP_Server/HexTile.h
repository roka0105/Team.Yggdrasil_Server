#pragma once
#include "GameObject.h"
class HexTile :public GameObject
{
public:
	enum class ETileType
	{
		None,
		DisabledBlock,
		NomalBlock,
		Max
	};
private:
	static int count;
public:
	HexTile(Vector3 _senter,float _radius)
	{
		m_senter = _senter;
		m_radius = _radius;
		m_id = count++;

	}
	~HexTile()
	{

	}
	bool InHex(Vector3 _objpos);
	Vector3 GetSenterPos();
	void operator=(UINT _time);
private:
	int m_id;
	UINT m_time;
	Vector3 m_senter;
	float m_radius;
	list<GameObject*> m_objects;
};

