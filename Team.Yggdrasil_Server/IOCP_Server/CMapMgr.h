#pragma once
class HexTile;
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
private:
	list<HexTile*> m_tiles;

};

