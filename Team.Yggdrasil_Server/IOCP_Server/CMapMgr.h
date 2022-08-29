#pragma once
class HexTile;
class CLock;
struct t_MapInfo
{
	t_MapInfo()
	{

	}
	t_MapInfo(UINT _id,Vector3 _start,Vector3 _end,float _tile_distance,
		      int _squared,int _depth,int _eyesight,float _defualty)
	{
		m_id = _id;
		m_start_position = _start;
		m_end_position = _end;
		m_tile_distance = _tile_distance;
		// 맵이 시작지점에서 종료지점까지 x 증가 z 감소
		float x = (m_start_position.x < 0 ? abs(m_start_position.x) : -m_start_position.x)
			     +(m_end_position.x);
		float z = abs((m_start_position.z > 0 ? -m_start_position.z : m_start_position.z)+ (m_end_position.z));
		m_h_mapsize = x;
		m_v_mapsize = z;
		m_squared_value = _squared;
		m_sector_count = 1;
		m_depth = _depth;
		m_eyesight = _eyesight;
		m_default_y = _defualty;
		
		m_start_position.y = m_default_y;
		m_end_position.y = m_default_y;
		for (int i = 0; i < m_depth; i++)
		{
			m_sector_count *= m_squared_value;
		}
		m_h_distance = (m_h_mapsize / m_sector_count);
		m_v_distance = (m_v_mapsize / m_sector_count);
	}
	
	UINT m_id;
	Vector3 m_start_position;
	Vector3 m_end_position;
	float m_tile_distance;// 육각형 타일 가로 세로 길이
	int m_h_mapsize;
	int m_v_mapsize;
	int m_sector_count;//현재 몇개의 섹터로 나뉜지 총 섹터 갯수
	int m_squared_value; // 몇등분 할 것인지. m_depth만큼 곱할것임. ex  squared=2 depth=2 => 4 가로세로 4등분. 
	int m_depth; // tree의 깊이
	int m_eyesight;
	float m_h_distance;
	float m_v_distance;
	float m_default_y;
};
class CMapMgr
{
public:
	static CMapMgr* GetInst();
	static void Create();
	static void Destroy();
private:
	static CMapMgr* m_instance;
public:
	CMapMgr();
	~CMapMgr();
	void Init();
	void End();
	t_MapInfo* GetMapInfo(UINT _mapid);

private:
	CLock* m_lock;
	list<HexTile*> m_tiles;
	map<UINT,t_MapInfo*> m_maps;
};

